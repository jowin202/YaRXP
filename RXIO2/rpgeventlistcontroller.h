#ifndef RPGEVENTLISTCONTROLLER_H
#define RPGEVENTLISTCONTROLLER_H

#include <QObject>
#include <QAction>
#include <QListWidget>
#include <QSettings>
#include <QJsonParseError>

class RPGDB;
class RPGMapController;
class RPGMapInfoController;

class RPGEventListController : public QObject
{
    Q_OBJECT
public:
    RPGEventListController(RPGMapController *mc, QListWidget *listwidget);





    void fill_list(QJsonArray list);
    QJsonArray get_list();
    QString get_text(QJsonObject obj);

private:


    RPGDB *db;
    RPGMapController *mc;
    RPGMapInfoController *mic;
    QListWidget *listwidget;

    QAction action_delete;
    QAction action_copy;
    QAction action_paste;

    int old_min = -1;
    int old_max = -1;

signals:


};

#endif // RPGEVENTLISTCONTROLLER_H
