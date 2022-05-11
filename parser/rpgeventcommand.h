#ifndef RPGEVENTCOMMAND_H
#define RPGEVENTCOMMAND_H

#include <QDebug>
#include <QObject>
#include <QVariant>

#include "rpgaudiofile.h"

class RPGEventCommand : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventCommand(QObject *parent = nullptr);
    void setParameter(QString param, int val);
    void setParameter(QString param, QList<QVariant> val);

    void debug();
    QString getType();

    int code;
    int indent;
    QList<QVariant> parameters;
    QString type;
    RPGAudioFile *audiofile;
    QStringList choices_list;

signals:

private:

};

#endif // RPGEVENTCOMMAND_H
