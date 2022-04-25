#ifndef RPGEVENTCOMMAND_H
#define RPGEVENTCOMMAND_H

#include <QObject>

class RPGEventCommand : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventCommand(QObject *parent = nullptr);

signals:

private:
    int code;
    int indent;
    QStringList parameters;

};

#endif // RPGEVENTCOMMAND_H
