#ifndef EVENTSCRIPTSERIALIZER_H
#define EVENTSCRIPTSERIALIZER_H

#include <QString>
#include <QJsonArray>

class EventScriptSerializer
{
public:
    static QString toYaml(const QJsonArray& commands);
    static QJsonArray fromYaml(const QString& yaml, bool* ok = nullptr, QString* errorMsg = nullptr);

    static QString escapeString(const QString& s);
};

#endif // EVENTSCRIPTSERIALIZER_H
