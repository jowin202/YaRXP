#ifndef RPGEDITORCONTROLLER_H
#define RPGEDITORCONTROLLER_H

#include <QObject>
#include <QLineEdit>
#include <QSpinBox>
#include <QListWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include "editors/checkboxlist.h"

#include <QDebug>
#include <QMetaMethod>

class RPGDB;
class RPGEditorController : public QObject
{
    Q_OBJECT
public:
    explicit RPGEditorController(RPGDB *db);


    void fill_combo(QComboBox *combo, int type, bool shownum, int chars, bool allow_none);


    //standard fields
    void connect_string_to_text_field(int object_type, QString key, QLineEdit *lineEdit);
    void connect_int_to_spin_box(int object_type, QString key, QSpinBox *spinbox);
    void connect_bool_to_check_box(int object_type, QString key, QCheckBox *checkbox);
    void connect_int_to_combo_box(int object_type, QString key, QComboBox *combo);
    void connect_int_to_data_combo_box(int object_type, QString key, QComboBox *combo, int content_type, bool shownum, int chars, bool allow_none);
    void connect_array_to_checkbox_list(int object_type, QString key, CheckBoxList *list, int content_type);



    void set_current_object(int object_type, int object_value);

    QJsonValue obj_get_jsonvalue(int obj_type, QString key);
    QStringList obj_get_name_list(int obj_type);


    void set_files(
            QJsonDocument actor_file,
            QJsonDocument animation_file,
            QJsonDocument armor_file,
            QJsonDocument class_file,
            QJsonDocument common_event_file,
            QJsonDocument enemy_file,
            QJsonDocument item_file,
            QJsonDocument skill_file,
            QJsonDocument state_file,
            QJsonDocument system_file,
            QJsonDocument tileset_file,
            QJsonDocument troop_file,
            QJsonDocument weapon_file);


    int current_actor = -1;
    int current_animation = -1;
    int current_armor = -1;
    int current_class = -1;
    int current_common_event = -1;
    int current_enemy = -1;
    int current_item = -1;
    int current_skill = -1;
    int current_state = -1;
    int current_system = -1;
    int current_tileset = -1;
    int current_troop = -1;
    int current_weapon = -1;

    bool block_writing = true;

    RPGDB *get_db(){return db;}


public slots:
    void obj_set_jsonvalue(int obj_type, QString key, QJsonValue value);
    //void obj_set_value(int obj_type, QString key, QVariant value);
    //void obj_set_array(int obj_type, QString key, QJsonArray value);
    void current_actor_set_parameters(QJsonArray params);


signals:
    void current_actor_changed();
    void current_animation_changed();
    void current_armor_changed();
    void current_class_changed();
    void current_common_event_changed();
    void current_enemy_changed();
    void current_item_changed();
    void current_skill_changed();
    void current_state_changed();
    void current_system_changed();
    void current_tileset_changed();
    void current_troop_changed();
    void current_weapon_changed();

    void current_actor_exp_curve(int,int);
    void current_actor_parameters(QJsonObject);



private:
    RPGDB *db;

    QJsonDocument actor_file;
    QJsonDocument animation_file;
    QJsonDocument armor_file;
    QJsonDocument class_file;
    QJsonDocument common_event_file;
    QJsonDocument enemy_file;
    QJsonDocument item_file;
    QJsonDocument skill_file;
    QJsonDocument state_file;
    QJsonDocument system_file;
    QJsonDocument tileset_file;
    QJsonDocument troop_file;
    QJsonDocument weapon_file;

    QMap<int,QMetaMethod> object_changed_signals;

    //same order as the enum in RPGDB!!!
    //enum {ACTORS, CLASSES, SKILLS, ITEMS, WEAPONS, ARMORS, ENEMIES, TROOPS, STATES, ANIMATIONS, TILESETS, COMMONEVENTS, SYSTEM};
    void (RPGEditorController::* obj_changed_signals[13])() = {
            &RPGEditorController::current_actor_changed,
            &RPGEditorController::current_class_changed,
            &RPGEditorController::current_skill_changed,
            &RPGEditorController::current_item_changed,
            &RPGEditorController::current_weapon_changed,
            &RPGEditorController::current_armor_changed,
            &RPGEditorController::current_enemy_changed,
            &RPGEditorController::current_troop_changed,
            &RPGEditorController::current_state_changed,
            &RPGEditorController::current_animation_changed,
            &RPGEditorController::current_tileset_changed,
            &RPGEditorController::current_common_event_changed,
            &RPGEditorController::current_system_changed,

                                                               };
};

#endif // RPGEDITORCONTROLLER_H
