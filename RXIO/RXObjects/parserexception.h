#ifndef PARSEREXCEPTION_H
#define PARSEREXCEPTION_H

#include <QObject>
#include <QVariant>
//#include "../settings.h"

class ParserException : public QObject
{
    Q_OBJECT
public:
    explicit ParserException(QString message, QVariantList error_data);


    QString message;
    QVariantList error_data;

signals:

};

#endif // PARSEREXCEPTION_H
