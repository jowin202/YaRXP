#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <QWidget>


#include "RXIO/RXObjects/rpgevent.h"
#include "RXIO/RXObjects/rpgeventpage.h"
#include "RXIO/RXObjects/rpgeventcommand.h"

#include "RXIO/RXObjects/rpgsettings.h"
#include "eventcommanddialog.h"


namespace Ui {
class EventPage;
}

class EventPage : public QWidget
{
    Q_OBJECT

public:
    explicit EventPage(QWidget *parent = nullptr, RPGEventPage *page = 0, RPGSettings *settings = 0);
    ~EventPage();

private slots:
    void move_type_changed(int val);

    void on_button_add_command_clicked();

private:
    Ui::EventPage *ui;
    RPGEventPage *page;
};

#endif // EVENTPAGE_H
