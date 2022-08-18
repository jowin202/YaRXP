#ifndef EVENTLIST_H
#define EVENTLIST_H

#include <QObject>
#include <QWidget>
#include <QDebug>

#include <QListWidget>

#include "../RXIO/RXObjects/rpgeventcommand.h"
#include "../RXIO/RXObjects/rpgsystem.h"


#include "commands/showtext.h"

class EventList : public QListWidget
{
    Q_OBJECT
public:
    EventList(QWidget *parent = 0);


    void set_data(RPGSystem *system, QList<RPGEventCommand*> *list)
    { this->system = system; this->list = list; }

    RPGSystem *system;
    QList<RPGEventCommand*> *list;




    void fill_list();


};


#endif // EVENTLIST_H
