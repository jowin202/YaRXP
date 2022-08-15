#ifndef RPGEVENTCOMMAND_H
#define RPGEVENTCOMMAND_H

#include <QDebug>
#include <QObject>
#include <QVariant>

#include "rpgvariant.h"
#include "rpgaudiofile.h"
#include "rpgstring.h"
#include "rpgmoveroute.h"

class RPGEventCommand : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventCommand(QObject *parent = nullptr);

    void debug();
    QString getType();

    int code;
    int indent;
    QList<RPGVariant> parameters;
    RPGString type;
    RPGAudioFile audiofile;
    RPGMoveRoute move_route;
    QList<RPGString> choices_list; //code 102 only

    //color tone
    double red, green, blue, gray;

    QStringList param_order;
signals:

private:

};

#endif // RPGEVENTCOMMAND_H
