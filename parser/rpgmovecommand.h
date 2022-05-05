#ifndef RPGMOVECOMMAND_H
#define RPGMOVECOMMAND_H

#include <QObject>

class RPGMoveCommand : public QObject
{
    Q_OBJECT
public:
    explicit RPGMoveCommand(QObject *parent = nullptr);
    void setParameter(QString param, int val);
    int code;

signals:
private:
    //parameters
};

#endif // RPGMOVECOMMAND_H
