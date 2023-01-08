#include "iomapinfofile.h"
#include "iotilesetfile.h"
#include "iosystemfile.h"
#include "ioactorfile.h"
#include "ioclassfile.h"
#include "ioskillfile.h"
#include "ioitemfile.h"
#include "ioarmorfile.h"
#include "ioweaponfile.h"
#include "ioenemyfile.h"
#include "iotroopfile.h"
#include "iostatefile.h"
#include "iocommoneventfile.h"
#include "ioanimationfile.h"

#include "iomapfile.h"

#include "iorgssad1.h"
#include "RXObjects/parserexception.h"

#include "testcases.h"

#include <QTemporaryFile>

#include "events/eventlist.h"

Testcases::Testcases(QObject *parent) : QObject(parent)
{

}

Testcases::Testcases(RPGSystem *settings)
{
    if (settings->data_dir == "") return;

    EventList event_list;

    this->ok = true;
    QByteArray hash1, hash2;
    QTemporaryFile tmpfile;
    tmpfile.setAutoRemove(true);
    tmpfile.open();
    tmpfile.close();


    qDebug() << "Checking System...";
    //Check system
    RPGSystem system;
    IOSystemFile system_file(settings->data_dir + "System.rxdata", &system);
    hash1 = system_file.getHash();

    system_file.write_to_file(tmpfile.fileName(), &system);
    hash2 = system_file.getHash();

    qDebug() << "System..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;



    qDebug() << "Checking MapInfo...";
    //Check MapInfo
    IOMapInfoFile info_file(settings->data_dir + "MapInfos.rxdata", &system.map_info_list);
    hash1 = info_file.getHash();

    info_file.write_to_file(tmpfile.fileName(), &system.map_info_list);
    hash2 = info_file.getHash();

    qDebug() << "MapInfos..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;


    QHash<int,QString> map_id_name;
    QHash<int,int> parent;
    for (int i = 0; i < system.map_info_list.length(); i++)
    {
        map_id_name.insert(system.map_info_list.at(i)->id, system.map_info_list.at(i)->name);
        parent.insert(system.map_info_list.at(i)->id, system.map_info_list.at(i)->parent_id);
    }



    qDebug() << "Checking Items...";
    //Check Items
    IOItemFile item_file(settings->data_dir + "Items.rxdata", &system.items_list);
    hash1 = item_file.getHash();


    item_file.write_to_file(tmpfile.fileName(), &system.items_list);
    hash2 = item_file.getHash();


    qDebug() << "Items..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;


    qDebug() << "Checking Animations...";
    //Check Animations
    IOAnimationFile animations_file(settings->data_dir + "Animations.rxdata", &system.animation_list);
    hash1 = animations_file.getHash();


    animations_file.write_to_file(tmpfile.fileName(), &system.animation_list);
    hash2 = animations_file.getHash();


    qDebug() << "Animations..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;


    qDebug() << "Checking Actors...";
    //Check Actors
    IOActorFile actor_file(settings->data_dir + "Actors.rxdata", &system.actor_list);
    hash1 = actor_file.getHash();


    actor_file.write_to_file(tmpfile.fileName(), &system.actor_list);
    hash2 = actor_file.getHash();


    qDebug() << "Actors..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;



    qDebug() << "Checking Skills...";
    //Check Skills
    IOSKillFile skill_file(settings->data_dir + "Skills.rxdata", &system.skills_list);
    hash1 = skill_file.getHash();



    skill_file.write_to_file(tmpfile.fileName(), &system.skills_list);
    hash2 = skill_file.getHash();



    qDebug() << "Skills..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;


    qDebug() << "Checking Classes...";
    //Check Classes
    IOClassFile class_file(settings->data_dir + "Classes.rxdata", &system.classes_list);
    hash1 = class_file.getHash();


    class_file.write_to_file(tmpfile.fileName(), &system.classes_list);
    hash2 = class_file.getHash();


    qDebug() << "Classes..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;




    qDebug() << "Checking Armor...";
    //Check Armor
    IOArmorFile armor_file(settings->data_dir + "Armors.rxdata", &system.armors_list);
    hash1 = armor_file.getHash();


    armor_file.write_to_file(tmpfile.fileName(), &system.armors_list);
    hash2 = armor_file.getHash();


    qDebug() << "Armors..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;



    qDebug() << "Checking Weapons...";
    //Check Weapons
    IOWeaponFile weapon_file(settings->data_dir + "Weapons.rxdata", &system.weapons_list);
    hash1 = weapon_file.getHash();


    weapon_file.write_to_file(tmpfile.fileName(), &system.weapons_list);
    hash2 = weapon_file.getHash();


    qDebug() << "Weapons..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;




    qDebug() << "Checking Enemies...";
    //Check Enemies
    IOEnemyFile enemy_file(settings->data_dir + "Enemies.rxdata", &system.enemies_list);
    hash1 = enemy_file.getHash();


    enemy_file.write_to_file(tmpfile.fileName(), &system.enemies_list);
    hash2 = enemy_file.getHash();


    qDebug() << "Enemies..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;





    qDebug() << "Checking Troops...";
    //Check Troops
    IOTroopFile troop_file(settings->data_dir + "Troops.rxdata", &system.troops_list);
    hash1 = troop_file.getHash();


    troop_file.write_to_file(tmpfile.fileName(), &system.troops_list);
    hash2 = troop_file.getHash();


    qDebug() << "Troops..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;



    qDebug() << "Checking States...";
    //Check States
    IOStateFile state_file(settings->data_dir + "States.rxdata", &system.states_list);
    hash1 = state_file.getHash();


    state_file.write_to_file(tmpfile.fileName(), &system.states_list);
    hash2 = state_file.getHash();


    qDebug() << "States..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;



    qDebug() << "Checking CommonEvents...";
    //Check CommonEvents
    QList<RPGCommonEvent*> common_event_list;
    IOCommonEventFile common_event_file(settings->data_dir + "CommonEvents.rxdata", &system.common_events_list);
    hash1 = common_event_file.getHash();


    common_event_file.write_to_file(tmpfile.fileName(), &system.common_events_list);
    hash2 = common_event_file.getHash();


    qDebug() << "Common Events..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;



    //Check Maps
    int num_maps = 0;
    int passed = 0;

    for (int k = 0; k < system.map_info_list.length(); k++)
    {
        int i = system.map_info_list.at(k)->id;

        QString filler;
        if (i >= 0 && i <= 9) filler = "00";
        else if (i >= 10 && i <= 99) filler = "0";
        else filler = "";

        RPGMap *map = system.map_info_list.at(k)->map = new RPGMap;
        QString path = settings->data_dir + "Map" + filler + QString::number(i) + ".rxdata";
        if (QFile(path).exists())
        {
            num_maps++;
            IOMapFile mapfile(path, map);
            hash1 = mapfile.getHash();
            system.current_map_id = k; // simulate that map is currently seen

            //recursive path
            QString map_path = map_id_name.value(i);
            int par_id = i;
            do
            {
                par_id = parent.value(par_id);
                map_path = map_path.prepend(map_id_name.value(par_id) + ">>");
            }while(par_id != 0);


            qDebug() << "Map: " << i << map_path;
            for (int e = 0; e < map->events.length(); e++)
            {
                qDebug() << "Map: " << i << "Event: " << map->events.at(e)->id << "(" + map->events.at(e)->name + ")"
                         << "x: " << map->events.at(e)->x << "y: " << map->events.at(e)->y;
                for (int j = 0; j < map->events.at(e)->pages.length(); j++)
                {
                    event_list.clear();
                    event_list.set_data(&system, &map->events.at(e)->pages.at(j)->list);
                    event_list.import_list();
                }
            }

            mapfile.write_to_file("/tmp/test_map.rxdata", map);
            hash2 = mapfile.getHash();
            qDebug() << "Map " << (filler + QString::number(i)) << (hash1==hash2 ? "passed":"failed");

            if (hash1==hash2)
                passed++;
        }

    }


    qDebug() << "Testing Map parser" <<passed << "Maps of" << num_maps << "passed";
    if (passed != num_maps)
        ok = false;






    //Check Tilesets
    QHash<int,RPGTileset *> tileset_hash;
    QList<RPGTileset*> tileset_list;
    IOTilesetFile tileset_file(settings->data_dir + "Tilesets.rxdata", &tileset_hash, &tileset_list);
    hash1 = tileset_file.getHash();

    tileset_file.write_to_file(tmpfile.fileName(), &tileset_list);
    hash2 = tileset_file.getHash();

    qDebug() << "Tilesets..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;


}
