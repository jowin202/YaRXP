#ifndef EDITSKILLS_H
#define EDITSKILLS_H

#include <QWidget>


class RPGSystem;

namespace Ui {
class EditSkills;
}

class EditSkills : public QWidget
{
    Q_OBJECT

public:
    explicit EditSkills(QWidget *parent = nullptr);
    ~EditSkills();


    void setSystem(RPGSystem *system)
    {this->system = system;}

    void set_skill(int n);


private:
    Ui::EditSkills *ui;
    RPGSystem *system;
};

#endif // EDITSKILLS_H
