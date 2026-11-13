#include "rpgdb.h"
#include "parser.h"
#include "writer.h"
#include "rpgeditorcontroller.h"
#include "factory.h"
#include "fileopener.h"
#include "rxexception.h"

RPGDB::RPGDB(QObject *parent)
    : QObject(parent)
{
}

RPGDB::~RPGDB()
{
    this->close_project();
}

namespace {
//returns the existing file (case insensitive match) or the default path if it does not exist yet
QString data_file_path(const QString &data_dir, const QString &name)
{
    QString existing = FileOpener(data_dir, name).get_existing_file();
    return existing.isEmpty() ? data_dir + name : existing;
}

QString map_file_name(int id)
{
    return QString("Map%1.rxdata").arg(id,3,10,QChar('0'));
}
}

void RPGDB::load_project(QString dir)
{
    if (!dir.endsWith(QDir::separator()))
    {
        dir += QDir::separator();
    }

    QString new_data_dir = FileOpener(dir, "Data").get_existing_directory();
    if (new_data_dir.isEmpty())
        throw RXException("No Data directory found in " + dir);

    //parse everything into temporary documents first,
    //so a broken file does not leave a half loaded project behind
    auto parse = [](QJsonDocument *doc, const QString &path, bool base64 = false) { Parser parser(doc, path, base64); };

    QJsonDocument new_actor_file, new_animation_file, new_armor_file, new_class_file, new_common_event_file,
            new_enemy_file, new_item_file, new_skill_file, new_state_file, new_system_file, new_tileset_file,
            new_troop_file, new_weapon_file, new_script_file, new_mapinfo_file;

    parse(&new_actor_file,         data_file_path(new_data_dir, "Actors.rxdata"));
    parse(&new_animation_file,     data_file_path(new_data_dir, "Animations.rxdata"));
    parse(&new_armor_file,         data_file_path(new_data_dir, "Armors.rxdata"));
    parse(&new_class_file,         data_file_path(new_data_dir, "Classes.rxdata"));
    parse(&new_common_event_file,  data_file_path(new_data_dir, "CommonEvents.rxdata"));
    parse(&new_enemy_file,         data_file_path(new_data_dir, "Enemies.rxdata"));
    parse(&new_item_file,          data_file_path(new_data_dir, "Items.rxdata"));
    parse(&new_skill_file,         data_file_path(new_data_dir, "Skills.rxdata"));
    parse(&new_state_file,         data_file_path(new_data_dir, "States.rxdata"));
    parse(&new_system_file,        data_file_path(new_data_dir, "System.rxdata"));
    parse(&new_tileset_file,       data_file_path(new_data_dir, "Tilesets.rxdata"));
    parse(&new_troop_file,         data_file_path(new_data_dir, "Troops.rxdata"));
    parse(&new_weapon_file,        data_file_path(new_data_dir, "Weapons.rxdata"));
    parse(&new_script_file,        data_file_path(new_data_dir, "Scripts.rxdata"), true); //base64
    parse(&new_mapinfo_file,       data_file_path(new_data_dir, "MapInfos.rxdata"));

    QMap<int,QJsonDocument*> new_map_files;
    try
    {
        foreach (const QString& key, new_mapinfo_file.object().keys())
        {
            if (key == "RXClass") continue;
            int i = key.toInt();
            QString map_path = FileOpener(new_data_dir, map_file_name(i)).get_existing_file();
            if (!map_path.isEmpty())
            {
                QJsonDocument *doc = new QJsonDocument;
                new_map_files.insert(i,doc);
                Parser map_parser(doc, map_path);
            }
            else
            {
                qDebug() << new_data_dir + map_file_name(i) << "does not exist";
            }
        }
    }
    catch (const RXException &)
    {
        qDeleteAll(new_map_files);
        throw;
    }

    //everything parsed successfully, replace the current project
    this->close_project();

    this->actor_file = new_actor_file;
    this->animation_file = new_animation_file;
    this->armor_file = new_armor_file;
    this->class_file = new_class_file;
    this->common_event_file = new_common_event_file;
    this->enemy_file = new_enemy_file;
    this->item_file = new_item_file;
    this->skill_file = new_skill_file;
    this->state_file = new_state_file;
    this->system_file = new_system_file;
    this->tileset_file = new_tileset_file;
    this->troop_file = new_troop_file;
    this->weapon_file = new_weapon_file;
    this->script_file = new_script_file;
    this->mapinfo_file = new_mapinfo_file;
    this->map_files = new_map_files;

    this->project_dir = dir;
    this->data_dir = new_data_dir;
    this->audio_dir = FileOpener(this->project_dir, "Audio").get_existing_directory_or_create_it();

    this->bgm_dir = FileOpener(this->audio_dir, "BGM").get_existing_directory_or_create_it();
    this->bgs_dir = FileOpener(this->audio_dir, "BGS").get_existing_directory_or_create_it();
    this->me_dir = FileOpener(this->audio_dir, "ME").get_existing_directory_or_create_it();
    this->se_dir = FileOpener(this->audio_dir, "SE").get_existing_directory_or_create_it();
    this->graphics_dir = FileOpener(this->project_dir, "Graphics").get_existing_directory_or_create_it();
    this->tileset_dir = FileOpener(this->graphics_dir, "Tilesets").get_existing_directory_or_create_it();
    this->autotiles_dir = FileOpener(this->graphics_dir, "Autotiles").get_existing_directory_or_create_it();
    this->character_dir = FileOpener(this->graphics_dir, "Characters").get_existing_directory_or_create_it();
    this->battler_dir = FileOpener(this->graphics_dir, "Battlers").get_existing_directory_or_create_it();
    this->battleback_dir = FileOpener(this->graphics_dir, "Battlebacks").get_existing_directory_or_create_it();
    this->animations_dir = FileOpener(this->graphics_dir, "Animations").get_existing_directory_or_create_it();
    this->icons_dir = FileOpener(this->graphics_dir, "Icons").get_existing_directory_or_create_it();
    this->pictures_dir = FileOpener(this->graphics_dir, "Pictures").get_existing_directory_or_create_it();
    this->windowskins_dir = FileOpener(this->graphics_dir, "Windowskins").get_existing_directory_or_create_it();
    this->transitions_dir = FileOpener(this->graphics_dir, "Transitions").get_existing_directory_or_create_it();
    this->panoramas_dir = FileOpener(this->graphics_dir, "Panoramas").get_existing_directory_or_create_it();
    this->fogs_dir = FileOpener(this->graphics_dir, "Fogs").get_existing_directory_or_create_it();
    this->titles_dir = FileOpener(this->graphics_dir, "Titles").get_existing_directory_or_create_it();
    this->gameovers_dir = FileOpener(this->graphics_dir, "Gameovers").get_existing_directory_or_create_it();
}

void RPGDB::save_project()
{
    if (this->project_dir.isEmpty())
        return; //nothing loaded

    auto write = [](QJsonDocument *doc, const QString &path, bool base64 = false) { Writer writer(doc, path, base64); };

    write(&actor_file,         data_file_path(this->data_dir, "Actors.rxdata"));
    write(&animation_file,     data_file_path(this->data_dir, "Animations.rxdata"));
    write(&armor_file,         data_file_path(this->data_dir, "Armors.rxdata"));
    write(&class_file,         data_file_path(this->data_dir, "Classes.rxdata"));
    write(&common_event_file,  data_file_path(this->data_dir, "CommonEvents.rxdata"));
    write(&enemy_file,         data_file_path(this->data_dir, "Enemies.rxdata"));
    write(&item_file,          data_file_path(this->data_dir, "Items.rxdata"));
    write(&skill_file,         data_file_path(this->data_dir, "Skills.rxdata"));
    write(&state_file,         data_file_path(this->data_dir, "States.rxdata"));
    write(&system_file,        data_file_path(this->data_dir, "System.rxdata"));
    write(&tileset_file,       data_file_path(this->data_dir, "Tilesets.rxdata"));
    write(&troop_file,         data_file_path(this->data_dir, "Troops.rxdata"));
    write(&weapon_file,        data_file_path(this->data_dir, "Weapons.rxdata"));
    write(&script_file,        data_file_path(this->data_dir, "Scripts.rxdata"), true); //base64

    write(&mapinfo_file,       data_file_path(this->data_dir, "MapInfos.rxdata"));

    for (auto it = map_files.constBegin(); it != map_files.constEnd(); ++it)
        write(it.value(), data_file_path(this->data_dir, map_file_name(it.key())));
}

void RPGDB::close_project()
{
    qDeleteAll(this->map_files);
    this->map_files.clear();
    this->project_dir.clear();
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
    QJsonArray arr = this->tileset_file.array();
    if (id < 0 || id >= arr.size())
        return QJsonObject();
    return arr.at(id).toObject();
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
    QJsonArray actors = this->actor_file.array();
    if (actor_id < 0 || actor_id >= actors.size())
        return result;
    QJsonObject actor = actors.at(actor_id).toObject();
    int class_id = actor.value("@class_id").toInt();
    QJsonArray classes = this->class_file.array();
    if (class_id < 0 || class_id >= classes.size())
        return result;
    QJsonObject class_obj = classes.at(class_id).toObject();
    result.append(class_obj.value("@weapon_set").toArray());

    QJsonArray shield;
    QJsonArray helmet;
    QJsonArray armor;
    QJsonArray accessory;

    QJsonArray armor_file_arr = this->armor_file.array();
    for (int i = 0; i < class_obj.value("@armor_set").toArray().count(); i++)
    {
        int id = class_obj.value("@armor_set").toArray().at(i).toInt();
        if (id < 0 || id >= armor_file_arr.size())
            continue;
        QJsonObject obj = armor_file_arr.at(id).toObject();
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
