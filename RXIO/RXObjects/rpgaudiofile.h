#ifndef RPGAUDIOFILE_H
#define RPGAUDIOFILE_H

#include <QDebug>
#include <QObject>

#include "rpgstring.h"

class RPGAudioFile : public QObject
{
    Q_OBJECT
public:
    explicit RPGAudioFile(QObject *parent = nullptr);
    ~RPGAudioFile();

    void debug();

    int pitch;
    int volume;
    RPGString name; //file name


    QStringList param_order;
signals:

private:
};


#endif // RPGAUDIOFILE_H
