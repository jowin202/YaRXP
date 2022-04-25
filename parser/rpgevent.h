#ifndef RPGEVENT_H
#define RPGEVENT_H

#include <QObject>
#include "rpgeventpage.h"

class RPGEvent : public QObject
{
    Q_OBJECT
public:
    explicit RPGEvent(QObject *parent = nullptr);

signals:


private:
    int id;
    int x;
    int y;
    QString name;
    QList<RPGEventPage*> pages;
};

#endif // RPGEVENT_H
