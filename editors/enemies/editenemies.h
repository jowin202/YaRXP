#ifndef EDITENEMIES_H
#define EDITENEMIES_H

#include <QWidget>
#include <QTableWidgetItem>

namespace Ui {
class EditEnemies;
}

class RPGSystem;
class EditEnemies : public QWidget
{
    Q_OBJECT

public:
    explicit EditEnemies(QWidget *parent = nullptr);
    ~EditEnemies();


    void set_enemy(int n);

    void setSystem(RPGSystem *system)
    {this->system = system;}

private slots:
    void on_table_action_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::EditEnemies *ui;
    RPGSystem *system;
};

#endif // EDITENEMIES_H
