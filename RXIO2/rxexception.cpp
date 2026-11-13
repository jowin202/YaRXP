#include "rxexception.h"

RXException::RXException(QString message)
    : msg(message)
{
    qDebug() << message;
}
