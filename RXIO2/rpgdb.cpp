#include "rpgdb.h"
#include "parser.h"
#include "writer.h"
#include "rpgeditorcontroller.h"

RPGDB::RPGDB(QObject *parent)
    : QObject(parent)
{
}

void RPGDB::load_project(QString dir)
{
    if (!dir.endsWith(QDir::separator()))
    {
        dir += QDir::separator();
    }
    this->project_dir = dir;

    Parser actor_parser(&actor_file, &param_oders, dir + "Data" + QDir::separator() + "Actors.rxdata");
    Parser animation_parser(&animation_file, &param_oders, dir + "Data" + QDir::separator() + "Animations.rxdata");
    Parser armor_parser(&armor_file, &param_oders, dir + "Data" + QDir::separator() + "Armors.rxdata");
    Parser class_parser(&class_file, &param_oders, dir + "Data" + QDir::separator() + "Classes.rxdata");
    Parser common_event_parser(&common_event_file, &param_oders, dir + "Data" + QDir::separator() + "CommonEvents.rxdata");
    Parser enemy_parser(&enemy_file, &param_oders, dir + "Data" + QDir::separator() + "Enemies.rxdata");
    Parser item_parser(&item_file, &param_oders, dir + "Data" + QDir::separator() + "Items.rxdata");
    Parser skill_parser(&skill_file, &param_oders, dir + "Data" + QDir::separator() + "Skills.rxdata");
    Parser state_parser(&state_file, &param_oders, dir + "Data" + QDir::separator() + "States.rxdata");
    Parser system_parser(&system_file, &param_oders, dir + "Data" + QDir::separator() + "System.rxdata");
    Parser tileset_parser(&tileset_file, &param_oders, dir + "Data" + QDir::separator() + "Tilesets.rxdata");
    Parser troop_parser(&troop_file, &param_oders, dir + "Data" + QDir::separator() + "Troops.rxdata");
    Parser weapon_parser(&weapon_file, &param_oders, dir + "Data" + QDir::separator() + "Weapons.rxdata");

    Parser mapinfo_parser(&mapinfo_file, &param_oders, dir + "Data" + QDir::separator() + "MapInfos.rxdata");


    for (int i = 0; i < 999; i++)
    {
        if (QFile(QString(dir + "Data" + QDir::separator() + "Map%1.rxdata").arg(i,3,10,QChar('0'))).exists())
        {
            QJsonDocument doc;
            Parser map_parser(&doc, &param_oders, QString(dir + "Data" + QDir::separator() + "Map%1.rxdata").arg(i,3,10,QChar('0')));
            map_files.insert(i,doc);
        }
    }


    /*
    QFile tmp("/tmp/actor.json");
    tmp.open(QIODevice::WriteOnly);
    tmp.write(actor_file.toJson());
    tmp.close();

    QFile tmp("/tmp/animation.json");
    tmp.open(QIODevice::WriteOnly);
    tmp.write(animation_file.toJson());
    tmp.close();
    */
}

void RPGDB::save_project()
{

    QString dir = "/tmp/rpg/";
    Writer actor_writer(&actor_file, &param_oders, dir + "Data" + QDir::separator() + "Actors.rxdata");
    Writer animation_writer(&animation_file, &param_oders, dir + "Data" + QDir::separator() + "Animations.rxdata");
    Writer armor_writer(&armor_file, &param_oders, dir + "Data" + QDir::separator() + "Armors.rxdata");
    Writer class_writer(&class_file, &param_oders, dir + "Data" + QDir::separator() + "Classes.rxdata");
    Writer common_event_writer(&common_event_file, &param_oders, dir + "Data" + QDir::separator() + "CommonEvents.rxdata");
    Writer enemy_writer(&enemy_file, &param_oders, dir + "Data" + QDir::separator() + "Enemies.rxdata");
    Writer item_writer(&item_file, &param_oders, dir + "Data" + QDir::separator() + "Items.rxdata");
    Writer skill_writer(&skill_file, &param_oders, dir + "Data" + QDir::separator() + "Skills.rxdata");
    Writer state_writer(&state_file, &param_oders, dir + "Data" + QDir::separator() + "States.rxdata");
    Writer system_writer(&system_file, &param_oders, dir + "Data" + QDir::separator() + "System.rxdata");
    Writer tileset_writer(&tileset_file, &param_oders, dir + "Data" + QDir::separator() + "Tilesets.rxdata");
    Writer troop_writer(&troop_file, &param_oders, dir + "Data" + QDir::separator() + "Troops.rxdata");
    Writer weapon_writer(&weapon_file, &param_oders, dir + "Data" + QDir::separator() + "Weapons.rxdata");

    Writer mapinfo_writer(&mapinfo_file, &param_oders, dir + "Data" + QDir::separator() + "MapInfos.rxdata");


    for (int i = 0; i < 999; i++)
    {
        if (map_files.contains(i))
        {
            QJsonDocument doc = map_files.value(i);
            Writer map_writer(&doc, &param_oders, QString(dir + "Data" + QDir::separator() + "Map%1.rxdata").arg(i,3,10,QChar('0')));
        }
    }



}

void RPGDB::close_project()
{

}

RPGEditorController* RPGDB::prepare_data_editor()
{
    RPGEditorController* ec = new RPGEditorController(this);
    ec->set_files(this->actor_file, this->animation_file, this->armor_file, this->class_file, this->common_event_file,
                        this->enemy_file, this->item_file, this->skill_file, this->state_file, this->system_file, this->tileset_file, this->troop_file, this->weapon_file);

    return ec;
}

void RPGDB::data_editor_data_saved()
{

}
