#include "rpgmovecommand.h"

RPGMoveCommand::RPGMoveCommand(QObject *parent) : QObject(parent)
{

}


void RPGMoveCommand::debug()
{
    qDebug() << "RPGMoveCommand: " << code;
}
