#ifndef RPGEVENT_H
#define RPGEVENT_H

#include <QObject>
#include <QDebug>
#include "rpgeventpage.h"

class RPGEvent : public QObject
{
    Q_OBJECT
public:
    explicit RPGEvent(QObject *parent = nullptr);
    ~RPGEvent();

    int id;
    int x;
    int y;
    RPGString name;
    QList<RPGEventPage*> pages;


    QStringList param_order;

    QImage get_event_pic();
signals:


private:

};

#endif // RPGEVENT_H
