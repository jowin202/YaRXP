#include "eventscriptserializer.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QStringList>

// ---------------------------------------------------------------------------
// Helpers: serialization
// ---------------------------------------------------------------------------

QString EventScriptSerializer::escapeString(const QString& s)
{
    QString r;
    r.reserve(s.size() + 8);
    for (QChar c : s) {
        if      (c == '\\') r += "\\\\";
        else if (c == '"')  r += "\\\"";
        else if (c == '\n') r += "\\n";
        else if (c == '\r') r += "\\r";
        else if (c == '\t') r += "\\t";
        else                r += c;
    }
    return r;
}

static QString scalarToYaml(const QJsonValue& val)
{
    if (val.isString())
        return "\"" + EventScriptSerializer::escapeString(val.toString()) + "\"";
    if (val.isBool())
        return val.toBool() ? "true" : "false";
    if (val.isNull() || val.isUndefined())
        return "null";
    if (val.isDouble()) {
        double d = val.toDouble();
        if (d >= (double)INT64_MIN && d <= (double)INT64_MAX && d == (long long)d)
            return QString::number((long long)d);
        return QString::number(d, 'g', 15);
    }
    if (val.isObject())
        return QString::fromUtf8(
            QJsonDocument(val.toObject()).toJson(QJsonDocument::Compact));
    // Nested array — caller handles this; shouldn't reach here
    return "null";
}

// ---------------------------------------------------------------------------
// toYaml
// ---------------------------------------------------------------------------

QString EventScriptSerializer::toYaml(const QJsonArray& commands)
{
    QString out;
    out.reserve(commands.size() * 64);
    out += "# YaRXP Event Script\n";

    for (const QJsonValue& cmdVal : commands) {
        QJsonObject cmd    = cmdVal.toObject();
        int code           = cmd.value("@code").toInt();
        int indent         = cmd.value("@indent").toInt();
        QJsonArray params  = cmd.value("@parameters").toArray();

        out += "- code: "   + QString::number(code)   + "\n";
        out += "  indent: " + QString::number(indent) + "\n";

        if (params.isEmpty()) {
            out += "  parameters: []\n";
        } else {
            out += "  parameters:\n";
            for (const QJsonValue& p : params) {
                if (p.isArray()) {
                    QJsonArray arr = p.toArray();
                    if (arr.isEmpty()) {
                        out += "    - []\n";
                    } else {
                        out += "    -\n";
                        for (const QJsonValue& item : arr)
                            out += "      - " + scalarToYaml(item) + "\n";
                    }
                } else {
                    out += "    - " + scalarToYaml(p) + "\n";
                }
            }
        }
    }

    return out;
}

// ---------------------------------------------------------------------------
// Helpers: parsing
// ---------------------------------------------------------------------------

static int lineIndent(const QString& line)
{
    int n = 0;
    for (QChar c : line) {
        if (c == ' ') n++;
        else break;
    }
    return n;
}

static QJsonValue parseScalar(const QString& raw)
{
    QString t = raw.trimmed();

    if (t.isEmpty() || t == "null" || t == "~")
        return QJsonValue::Null;
    if (t == "true")  return QJsonValue(true);
    if (t == "false") return QJsonValue(false);
    if (t == "[]")    return QJsonValue(QJsonArray());
    if (t == "{}")    return QJsonValue(QJsonObject());

    // Quoted string
    if (t.size() >= 2 && t.startsWith('"') && t.endsWith('"')) {
        QString inner = t.mid(1, t.size() - 2);
        QString result;
        result.reserve(inner.size());
        for (int j = 0; j < inner.size(); j++) {
            if (inner[j] == '\\' && j + 1 < inner.size()) {
                QChar next = inner[++j];
                if      (next == 'n')  result += '\n';
                else if (next == 'r')  result += '\r';
                else if (next == 't')  result += '\t';
                else if (next == '"')  result += '"';
                else if (next == '\\') result += '\\';
                else { result += '\\'; result += next; }
            } else {
                result += inner[j];
            }
        }
        return QJsonValue(result);
    }

    // Inline JSON object or array
    if (t.startsWith('{') || t.startsWith('[')) {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(t.toUtf8(), &err);
        if (err.error == QJsonParseError::NoError) {
            if (doc.isObject()) return QJsonValue(doc.object());
            if (doc.isArray())  return QJsonValue(doc.array());
        }
    }

    // Integer
    bool ok;
    qlonglong iv = t.toLongLong(&ok);
    if (ok) {
        if (iv >= INT_MIN && iv <= INT_MAX) return QJsonValue((int)iv);
        return QJsonValue((double)iv);
    }

    // Double
    double dv = t.toDouble(&ok);
    if (ok) return QJsonValue(dv);

    // Unquoted string fallback
    return QJsonValue(t);
}

// ---------------------------------------------------------------------------
// fromYaml
// ---------------------------------------------------------------------------

QJsonArray EventScriptSerializer::fromYaml(const QString& yaml, bool* ok, QString* errorMsg)
{
    QJsonArray result;

    QStringList lines = yaml.split('\n');
    while (!lines.isEmpty() && lines.last().trimmed().isEmpty())
        lines.removeLast();

    int lineCount = lines.size();
    int i = 0;

    auto setError = [&](const QString& msg) {
        if (ok)       *ok       = false;
        if (errorMsg) *errorMsg = msg;
    };

    // Advance past blank lines and comment lines
    auto skipBlank = [&]() {
        while (i < lineCount) {
            QString t = lines[i].trimmed();
            if (!t.isEmpty() && !t.startsWith('#'))
                break;
            i++;
        }
    };

    // Peek at next non-blank line index without advancing i
    auto peekNonBlank = [&](int from) -> int {
        int j = from;
        while (j < lineCount && lines[j].trimmed().isEmpty()) j++;
        return j;
    };

    while (i < lineCount) {
        skipBlank();
        if (i >= lineCount) break;

        // --- code ---
        const QString& codeLine = lines[i];
        if (!codeLine.startsWith("- code: ")) {
            setError(QString("Line %1: expected '- code: N', got: '%2'")
                         .arg(i + 1).arg(codeLine.trimmed()));
            return QJsonArray();
        }
        bool parseOk;
        int code = codeLine.mid(8).trimmed().toInt(&parseOk);
        if (!parseOk) {
            setError(QString("Line %1: invalid code value").arg(i + 1));
            return QJsonArray();
        }
        i++;

        // --- indent ---
        skipBlank();
        if (i >= lineCount) { setError("Unexpected end (expected indent)"); return QJsonArray(); }

        const QString& indentLine = lines[i];
        if (!indentLine.startsWith("  indent: ")) {
            setError(QString("Line %1: expected '  indent: N'").arg(i + 1));
            return QJsonArray();
        }
        int eventIndent = indentLine.mid(10).trimmed().toInt(&parseOk);
        if (!parseOk) {
            setError(QString("Line %1: invalid indent value").arg(i + 1));
            return QJsonArray();
        }
        i++;

        // --- parameters ---
        skipBlank();
        if (i >= lineCount) { setError("Unexpected end (expected parameters)"); return QJsonArray(); }

        const QString& paramsLine = lines[i];
        if (!paramsLine.startsWith("  parameters")) {
            setError(QString("Line %1: expected '  parameters'").arg(i + 1));
            return QJsonArray();
        }

        // Text after "  parameters"
        QString paramsRest = paramsLine.mid(12).trimmed();
        if (paramsRest.startsWith(':'))
            paramsRest = paramsRest.mid(1).trimmed();
        i++;

        QJsonArray params;

        if (paramsRest == "[]") {
            // empty — params stays empty
        } else if (paramsRest.isEmpty()) {
            // Block sequence: items at indent 4
            while (true) {
                int j = peekNonBlank(i);
                if (j >= lineCount) break;

                const QString& pl = lines[j];
                if (lineIndent(pl) < 4) break;

                QString trimPl = pl.trimmed();
                if (!trimPl.startsWith("- ") && trimPl != "-") break;

                i = j + 1;

                if (trimPl == "-") {
                    // Nested block sequence: items at indent 6
                    QJsonArray nested;
                    while (true) {
                        int k = peekNonBlank(i);
                        if (k >= lineCount) break;

                        const QString& nl = lines[k];
                        if (lineIndent(nl) < 6) break;

                        QString trimNl = nl.trimmed();
                        if (!trimNl.startsWith("- ") && trimNl != "-") break;

                        i = k + 1;
                        if (trimNl == "-")
                            nested.append(QJsonValue::Null);
                        else
                            nested.append(parseScalar(trimNl.mid(2)));
                    }
                    params.append(nested);
                } else {
                    // Scalar (or inline JSON)
                    params.append(parseScalar(trimPl.mid(2)));
                }
            }
        } else {
            // Inline value after the colon (edge case / manual editing)
            QJsonValue v = parseScalar(paramsRest);
            if (v.isArray())
                params = v.toArray();
            else
                params.append(v);
        }

        QJsonObject cmd;
        cmd.insert("@code",       code);
        cmd.insert("@indent",     eventIndent);
        cmd.insert("@parameters", params);
        cmd.insert("RXClass",     QString("RPG::EventCommand"));
        result.append(cmd);
    }

    if (ok) *ok = true;
    return result;
}
