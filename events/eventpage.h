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
class RPGMapController;
class RPGEventListController;

class EventPage : public QWidget
{
    Q_OBJECT

public:
    explicit EventPage(QJsonObject page, RPGMapController *mc, QWidget *parent = nullptr);
    EventPage(RPGMapController *mc, QWidget *parent = nullptr);
    ~EventPage();

    QJsonObject getPage();

private slots:
    void on_button_add_command_clicked();

    void on_combo_move_type_currentIndexChanged(int index);

    void on_button_move_route_clicked();

private:
    Ui::EventPage *ui;
    RPGMapController *mc;
    RPGDB *db;
    QJsonObject move_route;
    RPGEventListController *evc;
};

#endif // EVENTPAGE_H
