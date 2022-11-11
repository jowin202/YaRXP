#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <QWidget>


#include "RXIO/RXObjects/rpgevent.h"
#include "RXIO/RXObjects/rpgeventpage.h"
#include "RXIO/RXObjects/rpgeventcommand.h"

#include "eventcommanddialog.h"

namespace Ui {
class EventPage;
}


class RPGSystem;
class EventPage : public QWidget
{
    Q_OBJECT

public:
    explicit EventPage(QWidget *parent = nullptr, RPGEventPage *page = 0, RPGSystem *system = 0);
    ~EventPage();

    RPGEventPage *create_new_rpgeventpage();

private slots:
    void move_type_changed(int val);

    void on_button_add_command_clicked();

private:
    Ui::EventPage *ui;
    RPGSystem *system;


    //save this from rpgeventpage
    QStringList param_order;
    QStringList condition_param_order;
    QStringList graphic_param_order;
};

#endif // EVENTPAGE_H
