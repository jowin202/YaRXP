#ifndef RPGDB_H
#define RPGDB_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <QDir>


class RPGEditorController;

class RPGDB : public QObject
{
    Q_OBJECT
public:
    explicit RPGDB(QObject *parent = nullptr);

    void load_project(QString dir);
    void save_project();
    void close_project();

    enum {ACTORS, CLASSES, SKILLS, ITEMS, WEAPONS, ARMORS, ENEMIES, TROOPS, STATES, ANIMATIONS, TILESETS, COMMONEVENTS, SYSTEM};

    enum {CLASSES_WEAPONS = 1024, CLASSES_SHIELD, CLASSES_HELMET, CLASSES_BODY, CLASSES_ACCESSORY };

    //input output for data editor
    RPGEditorController *prepare_data_editor();
    void data_editor_data_saved();


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

    QJsonDocument mapinfo_file;
    QMap<int,QJsonDocument> map_files;


    QMap<QString, QStringList> param_oders;




signals:

};

#endif // RPGDB_H
