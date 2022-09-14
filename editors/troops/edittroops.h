#ifndef EDITTROOPS_H
#define EDITTROOPS_H

#include <QWidget>

namespace Ui {
class EditTroops;
}

class RPGSystem;
class EditTroops : public QWidget
{
    Q_OBJECT

public:
    explicit EditTroops(QWidget *parent = nullptr);
    ~EditTroops();

    void setSystem(RPGSystem *system)
    {this->system = system;}

    void set_troop(int n);

private:
    Ui::EditTroops *ui;
    RPGSystem *system;
};

#endif // EDITTROOPS_H
