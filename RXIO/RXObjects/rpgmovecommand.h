#ifndef RPGMOVECOMMAND_H
#define RPGMOVECOMMAND_H

#include <QObject>
#include <QDebug>
#include <QVariant>

#include "rpgaudiofile.h"
#include "rpgvariant.h"

class RPGMoveCommand : public QObject
{
    Q_OBJECT
public:
    explicit RPGMoveCommand(QObject *parent = nullptr);
    int code;

    void debug();
    QList<RPGVariant> parameters;
    RPGAudioFile audiofile;

signals:
private:
    //parameters
};

#endif // RPGMOVECOMMAND_H
