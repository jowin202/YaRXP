#ifndef RPGAUDIOFILE_H
#define RPGAUDIOFILE_H

#include <QObject>

class RPGAudioFile : public QObject
{
    Q_OBJECT
public:
    explicit RPGAudioFile(QObject *parent = nullptr);

signals:

private:
    int pitch;
    int volume;
    QString name; //file name
};

#endif // RPGAUDIOFILE_H
