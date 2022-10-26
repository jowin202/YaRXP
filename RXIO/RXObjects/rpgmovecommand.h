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

    QString get_command_text();


    QList<RPGVariant> parameters;
    RPGAudioFile audiofile;

    QStringList param_order;


    RPGMoveCommand *copy()
    {
        RPGMoveCommand *result = new RPGMoveCommand;
        result->code = this->code;
        this->audiofile.copy_to(&result->audiofile);
        result->param_order = QStringList(this->param_order);

        for (int i = 0; i < parameters.length(); i++)
            result->parameters.append(RPGVariant(this->parameters.at(i)));

        return result;
    }
signals:
private:
    //parameters
};

#endif // RPGMOVECOMMAND_H
