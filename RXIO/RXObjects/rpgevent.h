#ifndef RPGEVENT_H
#define RPGEVENT_H

#include <QObject>
#include "rpgeventpage.h"

class RPGEvent : public QObject
{
    Q_OBJECT
public:
    explicit RPGEvent(QObject *parent = nullptr);

    int id;
    int x;
    int y;
    RPGString name;
    QList<RPGEventPage*> pages;


signals:


private:

};

#endif // RPGEVENT_H
