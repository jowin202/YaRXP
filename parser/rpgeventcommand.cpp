#include "rpgeventcommand.h"

RPGEventCommand::RPGEventCommand(QObject *parent) : QObject(parent)
{

}

void RPGEventCommand::setParameter(QString param, int val)
{
    if (param == "@code")
        this->code = val;
    else if (param == "@indent")
        this->indent = val;
}

void RPGEventCommand::setParameter(QString param, QStringList val)
{
    if (param == "@parameters")
        this->parameters = val;
}
