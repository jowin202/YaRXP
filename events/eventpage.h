#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "eventcommanddialog.h"

namespace Ui {
class EventPage;
}


class RPGDB;
class RPGEventPageController;

class EventPage : public QWidget
{
    Q_OBJECT

public:
    explicit EventPage(QJsonObject page, RPGDB *db, QWidget *parent = nullptr);
    ~EventPage();

    QJsonObject getPage();

private slots:
    void move_type_changed(int val);
    void on_button_add_command_clicked();

private:
    Ui::EventPage *ui;
    RPGDB *db;
    QJsonObject page;
};

#endif // EVENTPAGE_H
