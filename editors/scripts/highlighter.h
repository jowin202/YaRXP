#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QTextDocument>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    QList<QRegularExpression> keyword_rules;
    QList<QRegularExpression> operator_rules;

    QTextCharFormat keywordFormat;
    QTextCharFormat operatorFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat RPGclassFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat stringFormat;

    const QString keywords[12] = {
        QStringLiteral("\\bdef\\b"), QStringLiteral("\\bend\\b"), QStringLiteral("\\breturn\\b"),
        QStringLiteral("\\bclass\\b"), QStringLiteral("\\belse\\b"), QStringLiteral("\\bmodule\\b"),
        QStringLiteral("\\bif\\b"), QStringLiteral("\\bsuper\\b"), QStringLiteral("\\bnil\\b"),
        QStringLiteral("\\btrue\\b"), QStringLiteral("\\bfalse\\b"), QStringLiteral("\\bself\\b"),
    };

    const QString operators[12] = {
        QStringLiteral("\\+"), QStringLiteral("\\-"), QStringLiteral("\\*"), QStringLiteral("\\/"),
        QStringLiteral("\\("), QStringLiteral("\\)"), QStringLiteral("<"), QStringLiteral(">"),
        QStringLiteral("="), QStringLiteral("!"), QStringLiteral("\\%"), QStringLiteral("\\."),
    };

};

#endif // HIGHLIGHTER_H
