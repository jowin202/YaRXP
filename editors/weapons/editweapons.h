#ifndef EDITWEAPONS_H
#define EDITWEAPONS_H

#include <QWidget>

namespace Ui {
class EditWeapons;
}

class RPGSystem;

class EditWeapons : public QWidget
{
    Q_OBJECT

public:
    explicit EditWeapons(QWidget *parent = nullptr);
    ~EditWeapons();


    void setSystem(RPGSystem *system)
    {this->system = system;}

    void set_weapon(int n);


private slots:

    void on_button_icon_clicked();

private:
    Ui::EditWeapons *ui;
    RPGSystem *system;
};

#endif // EDITWEAPONS_H
