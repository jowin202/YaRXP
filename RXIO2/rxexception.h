#ifndef RXEXCEPTION_H
#define RXEXCEPTION_H

#include <QDebug>
#include <QString>

class RXException
{
public:
    RXException(QString message);
    const QString &message() const { return this->msg; }

private:
    QString msg;
};

#endif // RXEXCEPTION_H
