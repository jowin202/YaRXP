#ifndef EDITANIMATIONS_H
#define EDITANIMATIONS_H

#include <QWidget>

namespace Ui {
class EditAnimations;
}

class RPGSystem;
class EditAnimations : public QWidget
{
    Q_OBJECT

public:
    explicit EditAnimations(QWidget *parent = nullptr);
    ~EditAnimations();

    void setSystem(RPGSystem *system)
    {this->system = system;}

    void set_animation(int n);


private:
    Ui::EditAnimations *ui;
    RPGSystem *system;
};

#endif // EDITANIMATIONS_H
