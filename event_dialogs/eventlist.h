#ifndef EVENTLIST_H
#define EVENTLIST_H

#include <QObject>
#include <QWidget>
#include <QDebug>

#include <QListWidget>

#include "../RXIO/RXObjects/rpgeventcommand.h"
#include "../RXIO/RXObjects/rpgsettings.h"


#include "commands/showtext.h"

class EventList : public QListWidget
{
    Q_OBJECT
public:
    EventList(QWidget *parent = 0);


    void set_data(RPGSettings *settings, QList<RPGEventCommand*> *list)
    { this->settings = settings; this->list = list; }

    RPGSettings *settings;
    QList<RPGEventCommand*> *list;




    void fill_list();


};


#endif // EVENTLIST_H
