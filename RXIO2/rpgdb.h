#ifndef RPGDB_H
#define RPGDB_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <QColor>
#include <QDir>
#include <QMap>

#include <QListWidget>
#include <QComboBox>


#include "factory.h"

class RPGEditorController;
class RPGMapController;

class RPGDB : public QObject
{
    Q_OBJECT
public:
    explicit RPGDB(QObject *parent = nullptr);

    RPGMapController *mc;
    void load_project(QString dir);
    void save_project();
    void close_project();

    enum {ACTORS, CLASSES, SKILLS, ITEMS, WEAPONS, ARMORS, ENEMIES, TROOPS, STATES, ANIMATIONS, TILESETS, COMMONEVENTS, SYSTEM, SCRIPTS};

    enum {CLASSES_WEAPONS = 1024, CLASSES_SHIELD, CLASSES_HELMET, CLASSES_BODY, CLASSES_ACCESSORY, ELEMENTS };

    //input output for data editor
    RPGEditorController *prepare_data_editor();

    //tileset_readonly
    QJsonObject get_tileset_by_id(int id);
    int add_tileset(QJsonObject tileset);

    //mapinfo changeable
    QJsonDocument *get_mapinfos() { return &this->mapinfo_file; }

    //map files changeable
    QJsonDocument *get_mapfile_by_id(int id) {
        if (this->map_files.contains(id))
            return this->map_files.value(id);
        else return 0;
    }
    void remove_map_file_by_id(int id){
        if (this->map_files.contains(id)){
            delete this->map_files.value(id);
            this->map_files.remove(id);
        }
    }
    QJsonDocument *create_mapfile_with_id(int id) {
        if (this->map_files.contains(id))
        {
            qDebug() << "Error: ID already exists";
            return 0;
        }
        this->map_files.insert(id, Factory().create_new_map());
        return this->map_files.value(id);
    }
    QJsonDocument *create_mapfile_from_object_with_id(int id, QJsonObject map) {
        if (this->map_files.contains(id))
        {
            qDebug() << "Error: ID already exists";
            return 0;
        }
        this->map_files.insert(id, new QJsonDocument(map));
        return this->map_files.value(id);
    }

    //switches and variables
    QJsonArray get_switch_names();
    QJsonArray get_variable_names();
    QString get_switch_name(int n) { return this->get_switch_names().at(n).toString();}
    QString get_variable_name(int n) { return this->get_variable_names().at(n).toString();}

    void change_switch_name(int i, QString name);
    void change_variable_name(int i, QString name);
    void set_max_switches(int n);
    void set_max_variables(int n);



    //Class Condition
    QJsonArray get_equipment_vars(int actor_id);



    QColor transparent = QColor(0x8a,0x16,0xa0);
    QString project_dir;
    QString data_dir;
    QString audio_dir;
    QString bgm_dir;
    QString bgs_dir;
    QString me_dir;
    QString se_dir;
    QString graphics_dir;
    QString tileset_dir;
    QString autotiles_dir;
    QString character_dir;
    QString battler_dir;
    QString battleback_dir;
    QString animations_dir;


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
            QJsonDocument weapon_file,
            QJsonDocument script_file);


    void fill_list(QListWidget *list, int type, bool shownum, int chars);
    void fill_combo(QComboBox *box, int type, bool shownum, int chars);
    QString get_object_name(int type, int id);
    int get_object_price(int type, int id);

private:
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
    QJsonDocument *files[14] = {&actor_file, &class_file, &skill_file, &item_file, &weapon_file, &armor_file, &enemy_file, &troop_file, &state_file, &animation_file, &tileset_file, &common_event_file, &system_file, &script_file };

    QJsonDocument mapinfo_file;
    QMap<int,QJsonDocument*> map_files;





signals:

};

#endif // RPGDB_H
