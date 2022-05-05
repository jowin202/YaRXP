#include "rpgaudiofile.h"

RPGAudioFile::RPGAudioFile(QObject *parent) : QObject(parent)
{
}

void RPGAudioFile::setParameter(QString param, int val)
{
    if (param == "@pitch")
        this->pitch = val;
    else if (param == "@volume")
        this->volume = val;
}

void RPGAudioFile::setParameter(QString param, QString val)
{
    if (param == "@name")
        this->name = val;
}

void RPGAudioFile::debug()
{
    qDebug() << name << " " << volume << " " << pitch;
}
