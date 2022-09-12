#include "rpgaudiofile.h"

RPGAudioFile::RPGAudioFile(QObject *parent) : QObject(parent)
{
}

RPGAudioFile::~RPGAudioFile()
{
    //qDebug() << "RPGAudioFile deleted";
}

void RPGAudioFile::debug()
{
    qDebug() << name << " " << volume << " " << pitch;
}

void RPGAudioFile::copy_to(RPGAudioFile *target)
{
    target->name = this->name;
    target->pitch = this->pitch;
    target->volume = this->volume;
}
