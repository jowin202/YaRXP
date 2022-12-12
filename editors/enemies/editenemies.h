#ifndef EDITENEMIES_H
#define EDITENEMIES_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QAction>
#include <QMenu>
#include <QSettings>


namespace Ui {
class EditEnemies;
}

class RPGEditorController;

class EditEnemies : public QWidget
{
    Q_OBJECT

public:
    explicit EditEnemies(QWidget *parent = nullptr);
    ~EditEnemies();



    void setEC(RPGEditorController *ec);

    void set_readonly(QTableWidgetItem *item)
    { item->setFlags(item->flags() & ~Qt::ItemIsEditable); }


public slots:
    void update_actions_from_file();

    void change_action(int row, int turn_a, int turn_b, int hp,
                       int level, int switch_id, int kind,
                       int skill, int rating, int basic);


private slots:
    void on_table_action_itemDoubleClicked(QTableWidgetItem *item);
    void on_button_action_del_clicked();
    void on_button_action_add_clicked();

    void item_add();
    void item_edit();
    void item_delete();
    void item_copy();
    void item_paste();




    void on_table_action_customContextMenuRequested(const QPoint &pos);

private:
    Ui::EditEnemies *ui;
    RPGEditorController *ec;
    QStringList basic_actions = { "Attack", "Defend", "Escape", "Do Nothing" };


    QAction *action_add;
    QAction *action_edit;
    QAction *action_delete;
    QAction *action_copy;
    QAction *action_paste;

};

#endif // EDITENEMIES_H
