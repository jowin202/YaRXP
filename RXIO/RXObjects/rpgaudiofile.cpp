#include "rpgaudiofile.h"

RPGAudioFile::RPGAudioFile(QObject *parent) : QObject(parent)
{
}

void RPGAudioFile::debug()
{
    qDebug() << name << " " << volume << " " << pitch;
}
