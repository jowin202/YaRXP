#ifndef RPGEVENT_H
#define RPGEVENT_H

#include <QObject>
#include "rpgeventpage.h"

class RPGEvent : public QObject
{
    Q_OBJECT
public:
    explicit RPGEvent(QObject *parent = nullptr);

    void setParameter(QString param, int val);
    void setParameter(QString param, QString val);

    int id;
    int x;
    int y;
    QString name;
    QList<RPGEventPage*> pages;


signals:


private:

};

#endif // RPGEVENT_H
