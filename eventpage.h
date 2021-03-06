#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <QWidget>


#include "parser/rpgevent.h"
#include "parser/rpgeventpage.h"
#include "parser/rpgeventpagecondition.h"
#include "parser/rpgeventcommand.h"
#include "parser/rpgeventpagegraphic.h"

#include "settings.h"


namespace Ui {
class EventPage;
}

class EventPage : public QWidget
{
    Q_OBJECT

public:
    explicit EventPage(QWidget *parent = nullptr, RPGEventPage *page = 0, Settings *settings = 0);
    ~EventPage();

private slots:
    void move_type_changed(int val);

private:
    Ui::EventPage *ui;
    RPGEventPage *page;
};

#endif // EVENTPAGE_H
