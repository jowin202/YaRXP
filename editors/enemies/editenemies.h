#ifndef EDITENEMIES_H
#define EDITENEMIES_H

#include <QWidget>
#include <QTableWidgetItem>

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


    void set_enemy(int n);
    void save();


    void setEC(RPGEditorController *ec);

    void set_readonly(QTableWidgetItem *item)
    { item->setFlags(item->flags() & ~Qt::ItemIsEditable); }


public slots:
    void change_action(int row, int turn_a, int turn_b, int hp,
                       int level, int switch_id, int kind,
                       int skill, int rating, int basic);
    void setTreasure(int item_id, int weapon_id, int armor_id, int prob);

private slots:
    void on_table_action_itemDoubleClicked(QTableWidgetItem *item);

    void on_button_action_del_clicked();

    void on_button_action_add_clicked();


    void on_button_change_treasure_clicked();


private:
    Ui::EditEnemies *ui;
    RPGEditorController *ec;

};

#endif // EDITENEMIES_H
