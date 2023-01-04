#include "rpgdb.h"
#include "parser.h"
#include "writer.h"
#include "rpgeditorcontroller.h"
#include "factory.h"

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

    foreach (const QString& key, mapinfo_file.object().keys())
    {
        if (key == "RXClass") continue;
        int i = key.toInt();
        if (QFile(QString(dir + "Data" + QDir::separator() + "Map%1.rxdata").arg(i,3,10,QChar('0'))).exists())
        {
            QJsonDocument *doc = new QJsonDocument;
            Parser map_parser(doc, &param_oders, QString(dir + "Data" + QDir::separator() + "Map%1.rxdata").arg(i,3,10,QChar('0')));
            map_files.insert(i,doc);
        }
        else
        {
            qDebug() << QString(dir + "Data" + QDir::separator() + "Map%1.rxdata").arg(i,3,10,QChar('0')) << "does not exist";
        }
    }

    /*
    qDebug() << (actor_file.array().last().toObject() == Factory().create_new_actor(actor_file.array().last().toObject().value("@id").toInt()));
    qDebug() << (class_file.array().last().toObject() == Factory().create_new_class(class_file.array().last().toObject().value("@id").toInt(), system_file.object().value("@elements").toArray().count(), state_file.array().count()) );
    qDebug() << (skill_file.array().last().toObject() == Factory().create_new_skill(skill_file.array().last().toObject().value("@id").toInt()));
    qDebug() << (item_file.array().last().toObject() == Factory().create_new_item(item_file.array().last().toObject().value("@id").toInt()));
    qDebug() << (weapon_file.array().last().toObject() == Factory().create_new_weapon(weapon_file.array().last().toObject().value("@id").toInt()));
    qDebug() << (armor_file.array().last().toObject() == Factory().create_new_armor(armor_file.array().last().toObject().value("@id").toInt()));
    qDebug() << (enemy_file.array().last().toObject() == Factory().create_new_enemy(enemy_file.array().last().toObject().value("@id").toInt(), system_file.object().value("@elements").toArray().count(), state_file.array().count()) );
    qDebug() << (troop_file.array().last().toObject() == Factory().create_new_troop(troop_file.array().last().toObject().value("@id").toInt()) );
    qDebug() << (state_file.array().last().toObject() == Factory().create_new_state(state_file.array().last().toObject().value("@id").toInt()) );
    qDebug() << (animation_file.array().last().toObject() == Factory().create_new_animation(animation_file.array().last().toObject().value("@id").toInt()) );
    qDebug() << (tileset_file.array().last().toObject() == Factory().create_new_tileset(tileset_file.array().last().toObject().value("@id").toInt()) );
    qDebug() << (common_event_file.array().last().toObject() == Factory().create_new_commonevent(common_event_file.array().last().toObject().value("@id").toInt()) );
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
            QJsonDocument *doc = map_files.value(i);
            Writer map_writer(doc, &param_oders, QString(dir + "Data" + QDir::separator() + "Map%1.rxdata").arg(i,3,10,QChar('0')));
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

QJsonObject RPGDB::get_tileset_by_id(int id)
{
    return this->tileset_file.array().at(id).toObject();
}

QJsonArray RPGDB::get_switch_names()
{
    return system_file.object().value("@switches").toArray();
}

QJsonArray RPGDB::get_variable_names()
{
    return system_file.object().value("@variables").toArray();
}

void RPGDB::change_switch_name(int i, QString name)
{
    QJsonArray array = system_file.object().value("@switches").toArray();
    array.removeAt(i);
    array.insert(i,name);

    QJsonObject system_obj = system_file.object();
    system_obj.insert("@switches", array);
    system_file.setObject(system_obj);
}

void RPGDB::change_variable_name(int i, QString name)
{
    QJsonArray array = system_file.object().value("@variables").toArray();
    array.removeAt(i);
    array.insert(i,name);

    QJsonObject system_obj = system_file.object();
    system_obj.insert("@variables", array);
    system_file.setObject(system_obj);
}

void RPGDB::set_max_switches(int n)
{
    QJsonArray array = system_file.object().value("@switches").toArray();

    while (array.count() > n+1) { array.removeLast(); }
    while (array.count() < n+1) { array.append(""); }

    QJsonObject system_obj = system_file.object();
    system_obj.insert("@switches", array);
    system_file.setObject(system_obj);
}

void RPGDB::set_max_variables(int n)
{
    QJsonArray array = system_file.object().value("@variables").toArray();

    while (array.count() > n+1) { array.removeLast(); }
    while (array.count() < n+1) { array.append(""); }

    QJsonObject system_obj = system_file.object();
    system_obj.insert("@variables", array);
    system_file.setObject(system_obj);
}

void RPGDB::set_files(QJsonDocument actor_file, QJsonDocument animation_file, QJsonDocument armor_file, QJsonDocument class_file, QJsonDocument common_event_file, QJsonDocument enemy_file, QJsonDocument item_file, QJsonDocument skill_file, QJsonDocument state_file, QJsonDocument system_file, QJsonDocument tileset_file, QJsonDocument troop_file, QJsonDocument weapon_file)
{
    this->actor_file = actor_file;
    this->animation_file = animation_file;
    this->armor_file = armor_file;
    this->class_file = class_file;
    this->common_event_file = common_event_file;
    this->enemy_file = enemy_file;
    this->item_file = item_file;
    this->skill_file = skill_file;
    this->state_file = state_file;
    this->system_file = system_file;
    this->tileset_file = tileset_file;
    this->troop_file = troop_file;
    this->weapon_file = weapon_file;
}
