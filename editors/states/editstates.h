#ifndef EDITSTATES_H
#define EDITSTATES_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class EditStates;
}

class RPGSystem;

class EditStates : public QWidget
{
    Q_OBJECT

public:
    explicit EditStates(QWidget *parent = nullptr);
    ~EditStates();


    void setSystem(RPGSystem *system)
    {this->system = system;}

    void set_state(int n);
    void save();

private:
    Ui::EditStates *ui;
    RPGSystem *system;
    int current;
};

#endif // EDITSTATES_H
