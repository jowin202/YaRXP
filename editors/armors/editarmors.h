#ifndef EDITARMORS_H
#define EDITARMORS_H

#include <QWidget>

namespace Ui {
class EditArmors;
}

class RPGSystem;
class EditArmors : public QWidget
{
    Q_OBJECT

public:
    explicit EditArmors(QWidget *parent = nullptr);
    ~EditArmors();

    void set_armor(int n);

    void setSystem(RPGSystem *system)
    {this->system = system;}

private:
    Ui::EditArmors *ui;
    RPGSystem *system;
};

#endif // EDITARMORS_H
