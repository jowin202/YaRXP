#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <QWidget>

namespace Ui {
class EventPage;
}

class EventPage : public QWidget
{
    Q_OBJECT

public:
    explicit EventPage(QWidget *parent = nullptr);
    ~EventPage();

private:
    Ui::EventPage *ui;
};

#endif // EVENTPAGE_H
