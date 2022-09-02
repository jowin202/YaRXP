#ifndef CLASSCHECKBOX_H
#define CLASSCHECKBOX_H

#include <QWidget>
#include <QCheckBox>

class ClassCheckbox : public QCheckBox
{
    Q_OBJECT
public:
    ClassCheckbox();


    bool is_weapon = false;
    bool is_armor = false;

    int weapon_id = -1;
    int armor_id = -1;
};

#endif // CLASSCHECKBOX_H
