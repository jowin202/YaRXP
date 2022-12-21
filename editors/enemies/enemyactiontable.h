#ifndef ENEMYACTIONTABLE_H
#define ENEMYACTIONTABLE_H

#include <QTableWidget>
#include <QObject>
#include <QWidget>

#include <QMenu>
#include <QAction>

#include <QSettings>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class RPGDB;
class RPGEditorController;

class EnemyActionTable : public QTableWidget
{
    Q_OBJECT
public:
    EnemyActionTable(QWidget *parent);
    void setEC(RPGEditorController *ec) {this->ec = ec; }


    void set_readonly(QTableWidgetItem *item)
    { item->setFlags(item->flags() & ~Qt::ItemIsEditable); }


public slots:
    void item_add();
    void item_edit();
    void item_delete();
    void item_copy();
    void item_paste();
    void update_actions_from_file();
    void change_action(int row, int turn_a, int turn_b, int hp, int level, int switch_id, int kind, int skill, int rating, int basic);
    void on_customContextMenuRequested(const QPoint &pos);


private:
    RPGEditorController *ec;

    QAction *action_add;
    QAction *action_edit;
    QAction *action_delete;
    QAction *action_copy;
    QAction *action_paste;

    QStringList basic_actions = { "Attack", "Defend", "Escape", "Do Nothing" };

};

#endif // ENEMYACTIONTABLE_H
