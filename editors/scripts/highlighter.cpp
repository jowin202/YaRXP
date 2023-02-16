#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    //keywords and operators
    keywordFormat.setForeground(Qt::blue);
    operatorFormat.setForeground(QColor(0x33, 0x66, 0xFF));
    numberFormat.setForeground(Qt::darkRed);
    RPGclassFormat.setForeground(Qt::darkMagenta);
    commentFormat.setForeground(Qt::gray);
    stringFormat.setForeground(Qt::darkGreen);

    for (const QString &pattern : keywords) {
        keyword_rules.append(QRegularExpression(pattern));
    }
    for (const QString &pattern : operators) {
        operator_rules.append(QRegularExpression(pattern));
    }
}



void Highlighter::highlightBlock(const QString &text)
{
    for (const QRegularExpression &rule : std::as_const(keyword_rules)) {
        QRegularExpressionMatchIterator matchIterator = rule.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), keywordFormat);
        }
    }

    for (const QRegularExpression &rule : std::as_const(operator_rules)) {
        QRegularExpressionMatchIterator matchIterator = rule.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), operatorFormat);
        }
    }

    //numbers
    {
        QRegularExpressionMatchIterator matchIterator = QRegularExpression(QStringLiteral("[0-9]|(\\.[0-9])")).globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), numberFormat);
        }
    }

    //classes
    {
        QRegularExpressionMatchIterator matchIterator = QRegularExpression(QStringLiteral("\\bRPG::[A-Za-z::]+\\b")).globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), RPGclassFormat);
        }
    }

    //strings
    {
        QRegularExpressionMatchIterator matchIterator = QRegularExpression(QStringLiteral("\"(.*)\"")).globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), stringFormat);
        }
    }

    //comment
    {
        QRegularExpressionMatchIterator matchIterator = QRegularExpression(QStringLiteral("#[^\n]*")).globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), commentFormat);
        }
    }

    setCurrentBlockState(0);

    QRegularExpression comment_start = QRegularExpression(QStringLiteral("^=begin"));
    QRegularExpression comment_end = QRegularExpression(QStringLiteral("^=end"));

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(comment_start);


    while (startIndex >= 0) {

        QRegularExpressionMatch match = comment_end.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, commentFormat);
        startIndex = text.indexOf(comment_start, startIndex + commentLength);
    }
}

