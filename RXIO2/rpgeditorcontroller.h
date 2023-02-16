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
#include "editors/abclist.h"
#include "editors/plusminuslist.h"

#include "dialogs/imagedisplaywidget.h"

#include <QDebug>
#include <QMetaMethod>

class RPGDB;
class RPGEditorController : public QObject
{
    Q_OBJECT
public:
    explicit RPGEditorController(RPGDB *db);


    void fill_combo(QComboBox *combo, int type, bool shownum, int chars, bool allow_none);
    void fill_list(QListWidget *list, int type, bool shownum, int chars, bool allow_none);


    //standard fields
    void connect_string_to_text_field(int object_type, QString key, QLineEdit *lineEdit);
    void connect_int_to_spin_box(int object_type, QString key, QSpinBox *spinbox);
    void connect_bool_to_check_box(int object_type, QString key, QCheckBox *checkbox);
    void connect_int_to_combo_box(int object_type, QString key, QComboBox *combo);
    void connect_int_to_data_combo_box(int object_type, QString key, QComboBox *combo, int content_type, bool shownum, int chars, bool allow_none);
    void connect_array_to_checkbox_list(int object_type, QString key, CheckBoxList *list, int content_type);
    void connect_table_to_abc_list(int object_type, QString key, ABCList *list, int content_type);
    void connect_plus_minus_list(int object_type, QString key_plus, QString key_minus, PlusMinusList *list, int content_type);

    void connect_image_display_widget(int object_type, int image_type, QString key, QString key_hue, ImageDisplayWidget *widget);


    void set_current_object(int object_type, int object_value);


    QJsonValue obj_get_jsonvalue(int obj_type, QString key);
    QStringList obj_get_name_list(int obj_type);
    QJsonObject get_object_by_id(int obj_type, int id);
    void set_object_by_id(int obj_type, int id, QJsonValue obj);


    //scripts
    QJsonArray get_script_by_id(int id);
    void remove_script_by_id(int id);
    void add_new_script_at_id(int id);

    //max object and count
    int count_objects(int obj_type);
    void set_max(int obj_type, int count);

    void set_files(QJsonDocument actor_file,
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
            QJsonDocument weapon_file, QJsonDocument script_file);


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
    int current_script = -1;

    bool block_writing = true;
    bool block_changing_objects = false;

    RPGDB *get_db(){return db;}



public slots:
    void obj_set_jsonvalue(int obj_type, QString key, QJsonValue value);
    void current_actor_set_parameters(QJsonArray params);
    void refresh(int object_type);
    void save_edited_data();


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
    void current_script_changed();

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
    QJsonDocument script_file;

    QMap<int,QMetaMethod> object_changed_signals;

    //same order as the enum in RPGDB!!!
    //enum {ACTORS, CLASSES, SKILLS, ITEMS, WEAPONS, ARMORS, ENEMIES, TROOPS, STATES, ANIMATIONS, TILESETS, COMMONEVENTS, SYSTEM, SCRIPTS};
    void (RPGEditorController::* obj_changed_signals[14])() = {
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
            &RPGEditorController::current_script_changed,
                                                               };

    //0 for system
    int *current_instance_variables[14] = { &current_actor, &current_class, &current_skill, &current_item, &current_weapon, &current_armor, &current_enemy, &current_troop, &current_state, &current_animation, &current_tileset, &current_common_event, 0, &current_script};
    QJsonDocument *files[14] = {&actor_file, &class_file, &skill_file, &item_file, &weapon_file, &armor_file, &enemy_file, &troop_file, &state_file, &animation_file, &tileset_file, &common_event_file, &system_file, &script_file };

};

#endif // RPGEDITORCONTROLLER_H
