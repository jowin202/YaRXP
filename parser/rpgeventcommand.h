#ifndef RPGEVENTCOMMAND_H
#define RPGEVENTCOMMAND_H

#include <QObject>

class RPGEventCommand : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventCommand(QObject *parent = nullptr);
    void setParameter(QString param, int val);
    void setParameter(QString param, QStringList val);


    int code;
    int indent;
    QStringList parameters;

signals:

private:

};

#endif // RPGEVENTCOMMAND_H
