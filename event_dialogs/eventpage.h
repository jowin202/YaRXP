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
    QString character_name;

private slots:
    void move_type_changed(int val);

    void on_button_add_command_clicked();
    void open_page_graphic_dialog();

private:
    Ui::EventPage *ui;
    RPGSystem *system;
};

#endif // EVENTPAGE_H
