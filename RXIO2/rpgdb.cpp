#include "rpgdb.h"
#include "parser.h"
#include "writer.h"
#include "rpgeditorcontroller.h"
#include "factory.h"
#include "fileopener.h"

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
    this->data_dir = FileOpener(this->project_dir, "data").get_existing_directory();
    this->audio_dir = FileOpener(this->project_dir, "audio").get_existing_directory();

    this->audio_dir = FileOpener(this->project_dir, "audio").get_existing_directory();
    this->bgm_dir = FileOpener(this->audio_dir, "bgm").get_existing_directory();
    this->bgs_dir = FileOpener(this->audio_dir, "bgs").get_existing_directory();
    this->me_dir = FileOpener(this->audio_dir, "me").get_existing_directory();
    this->se_dir = FileOpener(this->audio_dir, "se").get_existing_directory();
    this->graphics_dir = FileOpener(this->project_dir, "graphics").get_existing_directory();
    this->tileset_dir = FileOpener(this->graphics_dir, "tilesets").get_existing_directory();
    this->autotiles_dir = FileOpener(this->graphics_dir, "autotiles").get_existing_directory();
    this->character_dir = FileOpener(this->graphics_dir, "characters").get_existing_directory();
    this->battler_dir = FileOpener(this->graphics_dir, "battlers").get_existing_directory();
    this->battleback_dir = FileOpener(this->graphics_dir, "battlebacks").get_existing_directory();
    this->animations_dir = FileOpener(this->graphics_dir, "animations").get_existing_directory();

    Parser actor_parser(&actor_file,                FileOpener(this->data_dir,"Actors.rxdata").get_existing_file());
    Parser animation_parser(&animation_file,        FileOpener(this->data_dir,"Animations.rxdata").get_existing_file());
    Parser armor_parser(&armor_file,                FileOpener(this->data_dir,"Armors.rxdata").get_existing_file());
    Parser class_parser(&class_file,                FileOpener(this->data_dir,"Classes.rxdata").get_existing_file());
    Parser common_event_parser(&common_event_file,  FileOpener(this->data_dir,"CommonEvents.rxdata").get_existing_file());
    Parser enemy_parser(&enemy_file,                FileOpener(this->data_dir,"Enemies.rxdata").get_existing_file());
    Parser item_parser(&item_file,                  FileOpener(this->data_dir,"Items.rxdata").get_existing_file());
    Parser skill_parser(&skill_file,                FileOpener(this->data_dir,"Skills.rxdata").get_existing_file());
    Parser state_parser(&state_file,                FileOpener(this->data_dir,"States.rxdata").get_existing_file());
    Parser system_parser(&system_file,              FileOpener(this->data_dir,"System.rxdata").get_existing_file());
    Parser tileset_parser(&tileset_file,            FileOpener(this->data_dir,"Tilesets.rxdata").get_existing_file());
    Parser troop_parser(&troop_file,                FileOpener(this->data_dir,"Troops.rxdata").get_existing_file());
    Parser weapon_parser(&weapon_file,              FileOpener(this->data_dir,"Weapons.rxdata").get_existing_file());
    Parser script_parser(&script_file,              FileOpener(this->data_dir,"Scripts.rxdata").get_existing_file(), true); //base64


    Parser mapinfo_parser(&mapinfo_file,            FileOpener(this->data_dir,"MapInfos.rxdata").get_existing_file());

    foreach (const QString& key, mapinfo_file.object().keys())
    {
        if (key == "RXClass") continue;
        int i = key.toInt();
        if (QFile(QString(this->data_dir + "Map%1.rxdata").arg(i,3,10,QChar('0'))).exists())
        {
            QJsonDocument *doc = new QJsonDocument;
            Parser map_parser(doc,  QString(this->data_dir + "Map%1.rxdata").arg(i,3,10,QChar('0')));
            map_files.insert(i,doc);
        }
        else
        {
            qDebug() << QString(this->data_dir + "Map%1.rxdata").arg(i,3,10,QChar('0')) << "does not exist";
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
    Writer actor_writer(&actor_file,                FileOpener(this->data_dir, "Actors.rxdata").get_existing_file());
    Writer animation_writer(&animation_file,        FileOpener(this->data_dir, "Animations.rxdata").get_existing_file());
    Writer armor_writer(&armor_file,                FileOpener(this->data_dir, "Armors.rxdata").get_existing_file());
    Writer class_writer(&class_file,                FileOpener(this->data_dir, "Classes.rxdata").get_existing_file());
    Writer common_event_writer(&common_event_file,  FileOpener(this->data_dir, "CommonEvents.rxdata").get_existing_file());
    Writer enemy_writer(&enemy_file,                FileOpener(this->data_dir, "Enemies.rxdata").get_existing_file());
    Writer item_writer(&item_file,                  FileOpener(this->data_dir, "Items.rxdata").get_existing_file());
    Writer skill_writer(&skill_file,                FileOpener(this->data_dir, "Skills.rxdata").get_existing_file());
    Writer state_writer(&state_file,                FileOpener(this->data_dir, "States.rxdata").get_existing_file());
    Writer system_writer(&system_file,              FileOpener(this->data_dir, "System.rxdata").get_existing_file());
    Writer tileset_writer(&tileset_file,            FileOpener(this->data_dir, "Tilesets.rxdata").get_existing_file());
    Writer troop_writer(&troop_file,                FileOpener(this->data_dir, "Troops.rxdata").get_existing_file());
    Writer weapon_writer(&weapon_file,              FileOpener(this->data_dir, "Weapons.rxdata").get_existing_file());
    Writer script_writer(&script_file,              FileOpener(this->data_dir, "Scripts.rxdata").get_existing_file(), true); //base64

    Writer mapinfo_writer(&mapinfo_file,            FileOpener(this->data_dir, "MapInfos.rxdata").get_existing_file());


    for (int i = 0; i < 999; i++)
    {
        if (map_files.contains(i))
        {
            QJsonDocument *doc = map_files.value(i);
            Writer map_writer(doc, QString(this->data_dir + "Map%1.rxdata").arg(i,3,10,QChar('0')));
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
                        this->enemy_file, this->item_file, this->skill_file, this->state_file, this->system_file, this->tileset_file, this->troop_file, this->weapon_file, this->script_file);

    return ec;
}

QJsonObject RPGDB::get_tileset_by_id(int id)
{
    return this->tileset_file.array().at(id).toObject();
}

int RPGDB::add_tileset(QJsonObject tileset)
{
    QJsonArray array = this->tileset_file.array();
    array.append(tileset);
    this->tileset_file.setArray(array);
    return array.count()-1; //returns new tileset id
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

QJsonArray RPGDB::get_equipment_vars(int actor_id)
{
    QJsonArray result;
    QJsonObject actor = this->actor_file.array().at(actor_id).toObject();
    QJsonObject class_obj = this->class_file.array().at(actor.value("@class_id").toInt()).toObject();
    result.append(class_obj.value("@weapon_set").toArray());

    QJsonArray shield;
    QJsonArray helmet;
    QJsonArray armor;
    QJsonArray accessory;

    for (int i = 0; i < class_obj.value("@armor_set").toArray().count(); i++)
    {
        int id = class_obj.value("@armor_set").toArray().at(i).toInt();
        QJsonObject obj = this->armor_file.array().at(id).toObject();
        if (obj.value("@kind").toInt() == 0)
            shield.append(id);
        else if (obj.value("@kind").toInt() == 1)
            helmet.append(id);
        else if (obj.value("@kind").toInt() == 2)
            armor.append(id);
        else if (obj.value("@kind").toInt() == 3)
            accessory.append(id);
    }
    result.append(shield);
    result.append(helmet);
    result.append(armor);
    result.append(accessory);


    return result;
}

void RPGDB::set_files(QJsonDocument actor_file, QJsonDocument animation_file, QJsonDocument armor_file, QJsonDocument class_file, QJsonDocument common_event_file, QJsonDocument enemy_file, QJsonDocument item_file, QJsonDocument skill_file, QJsonDocument state_file, QJsonDocument system_file, QJsonDocument tileset_file, QJsonDocument troop_file, QJsonDocument weapon_file, QJsonDocument script_file)
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
    this->script_file = script_file;
}

void RPGDB::fill_list(QListWidget *list, int type, bool shownum, int chars)
{
    list->clear();
    for (int i = 1; i < files[type]->array().count(); i++)
        list->addItem(shownum ? QString("%1: " + files[type]->array().at(i).toObject().value("@name").toString())
                                .arg(i,chars,10,QChar('0')) :
                                files[type]->array().at(i).toObject().value("@name").toString());

}

void RPGDB::fill_combo(QComboBox *box, int type, bool shownum, int chars)
{
    box->clear();
    for (int i = 1; i < files[type]->array().count(); i++)
        box->addItem(shownum ? QString("%1: " + files[type]->array().at(i).toObject().value("@name").toString())
                                .arg(i,chars,10,QChar('0')) :
                               files[type]->array().at(i).toObject().value("@name").toString(),QVariant(i));
}

QString RPGDB::get_object_name(int type, int id)
{
    return files[type]->array().at(id).toObject().value("@name").toString();
}

int RPGDB::get_object_price(int type, int id)
{
    return files[type]->array().at(id).toObject().value("@price").toInt();
}
