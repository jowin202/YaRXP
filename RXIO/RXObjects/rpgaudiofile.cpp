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
