#include "rpgmovecommand.h"

RPGMoveCommand::RPGMoveCommand(QObject *parent) : QObject(parent)
{

}


void RPGMoveCommand::debug()
{
    qDebug() << "RPGMoveCommand: " << code;
}

QString RPGMoveCommand::get_command_text()
{
    if (code == 1)
        return "$>Move Down";
    else if (code == 2)
        return "$>Move Left";
    else if (code == 3)
        return "$>Move Right";
    else if (code == 4)
        return "$>Move Up";
    else if (code == 5)
        return "$>Move Lower Left";
    else if (code == 6)
        return "$>Move Lower Right";
    else if (code == 7)
        return "$>Move Upper Left";
    else if (code == 8)
        return "$>Move Upper Right";
    else if (code == 9)
        return "$>Move at Random";
    else if (code == 10)
        return "$>Move toward Player";
    else if (code == 11)
        return "$>Move away from Player";
    else if (code == 12)
        return "$>1 Step Forward";
    else if (code == 13)
        return "$>1 Step Backward";
    else if (code == 14)
        return "$>Jump: " + QString::number(parameters.at(0).toInt()) + ","+ QString::number(parameters.at(1).toInt());
    else if (code == 15)
        return "$>Wait: " + QString::number(parameters.at(0).toInt()) + " frame(s)";

    if (code == 16)
        return "$>Turn Down";
    else if (code == 17)
        return "$>Turn Left";
    else if (code == 18)
        return "$>Turn Right";
    else if (code == 19)
        return "$>Turn Up";
    else if (code == 20)
        return "$>Turn 90 Right";
    else if (code == 21)
        return "$>Turn 90 Left";
    else if (code == 22)
        return "$>Turn 108";
    else if (code == 23)
        return "$>Turn 90 Right or Left";
    else if (code == 24)
        return "$>Turn at Random";
    else if (code == 25)
        return "$>Turn toward Player";
    else if (code == 26)
        return "$>Turn away from Player";
    else if (code == 27)
        return "$>Switch ON: " + QString::number(parameters.at(0).toInt());
    else if (code == 28)
        return "$>Switch OFF: " + QString::number(parameters.at(0).toInt());
    else if (code == 29)
        return "$>Change Speed: " + QString::number(parameters.at(0).toInt());
    else if (code == 30)
        return "$>Change Freq: " + QString::number(parameters.at(0).toInt());

    else if (code == 31)
        return "$>Move Animation ON";
    else if (code == 32)
        return "$>Move Animation OFF";
    else if (code == 33)
        return "$>Stop Animation ON";
    else if (code == 34)
        return "$>Stop Animation OFF";
    else if (code == 35)
        return "$>Direction Fix ON";
    else if (code == 36)
        return "$>Direction Fix OFF";
    else if (code == 37)
        return "$>Through ON";
    else if (code == 38)
        return "$>Through OFF";
    else if (code == 39)
        return "$>Always on Top ON";
    else if (code == 40)
        return "$>Always on Top OFF";
    else if (code == 41)
        return "$>Graphic: " + (parameters.at(0).str == "" ? "(None)" : QString(parameters.at(0).str));
    else if (code == 42)
        return "$>Change Opacity: " + QString::number(parameters.at(0).toInt());
    else if (code == 43)
    {
        QStringList type;
        type << "Normal" << "Add" << "Sub";
        return "$>Change Blending: " + type[parameters.at(0).toInt()];
    }
    else if (code == 44)
        return "$>SE: " + audiofile.name;
    else if (code == 45)
        return "$>Script: " + parameters.at(0).str;

    return QString();
}
