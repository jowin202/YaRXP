#ifndef RPGEVENTCOMMAND_H
#define RPGEVENTCOMMAND_H

#include <QObject>
#include <QVariant>

class RPGEventCommand : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventCommand(QObject *parent = nullptr);
    void setParameter(QString param, int val);
    void setParameter(QString param, QList<QVariant> val);


    int code;
    int indent;
    QList<QVariant> parameters;

signals:

private:

};

#endif // RPGEVENTCOMMAND_H
