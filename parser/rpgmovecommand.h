#ifndef RPGMOVECOMMAND_H
#define RPGMOVECOMMAND_H

#include <QObject>

class RPGMoveCommand : public QObject
{
    Q_OBJECT
public:
    explicit RPGMoveCommand(QObject *parent = nullptr);

signals:
private:
    int code;
    //parameters
};

#endif // RPGMOVECOMMAND_H
