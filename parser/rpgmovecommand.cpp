#include "rpgmovecommand.h"

RPGMoveCommand::RPGMoveCommand(QObject *parent) : QObject(parent)
{

}

void RPGMoveCommand::setParameter(QString param, int val)
{
    //if (param == "@code")
    this->code = val;
}
