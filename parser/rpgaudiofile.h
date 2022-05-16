#ifndef RPGAUDIOFILE_H
#define RPGAUDIOFILE_H

#include <QDebug>
#include <QObject>

class RPGAudioFile : public QObject
{
    Q_OBJECT
public:
    explicit RPGAudioFile(QObject *parent = nullptr);
    void debug();

    int pitch;
    int volume;
    QString name; //file name


signals:

private:
};


#endif // RPGAUDIOFILE_H
