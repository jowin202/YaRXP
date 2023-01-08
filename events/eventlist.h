#ifndef EVENTLIST_H
#define EVENTLIST_H

#include <QObject>
#include <QWidget>
#include <QDebug>

#include <QListWidget>

#include "../RXIO/RXObjects/rpgeventcommand.h"
#include "../RXIO/RXObjects/rpgsystem.h"



class EventList : public QListWidget
{
    Q_OBJECT
public:
    EventList(QWidget *parent = 0);


    void set_data(RPGSystem *system, QList<RPGEventCommand*> *list)
    { this->system = system; this->list = list; }

    RPGSystem *system;
    QList<RPGEventCommand*> *list;




    void import_list();
    void export_list();

    QStringList param_order;
public slots:
    void fix_selections();


};


#endif // EVENTLIST_H