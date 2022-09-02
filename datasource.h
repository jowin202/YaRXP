#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QObject>
#include <QListWidget>
#include <QComboBox>

class RPGSkill;
class RPGSystem;

class DataSource : public QObject
{
    Q_OBJECT
public:
    explicit DataSource(QObject *parent = nullptr);

    void setSystem(RPGSystem *system) {this->system = system;}
    enum {ACTORS, CLASSES, SKILLS, ITEMS, WEAPONS, ARMORS, ENEMIES, TROOPS, STATES, ANIMATIONS, TILESETS, COMMONEVENTS, SYSTEM};


    void fill_list(QListWidget *list, int type, bool shownum, int chars);
    void fill_combo(QComboBox *combo, int type, bool shownum, int chars, int current);

    void fill_combo_weapon_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current);
    void fill_combo_armor_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current, int amor_type);
    void fill_combo_shield_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current);
    void fill_combo_helmet_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current);
    void fill_combo_body_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current);
    void fill_combo_accessory_by_class(QComboBox *combo, int class_id, bool shownum, int chars, int current);


    QString get_obj_name_by_id(int id, int type, bool shownum, int chars);

signals:

private:
    RPGSystem *system;
};

#endif // DATASOURCE_H
