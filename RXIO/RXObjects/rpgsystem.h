#ifndef RPGSYSTEM_H
#define RPGSYSTEM_H

#include <QColor>
#include <QObject>
#include "rpgaudiofile.h"
#include "rpgtileset.h"
#include "rpgstring.h"
#include "rpgmapinfo.h"
#include "rpgactor.h"
#include "rpgclass.h"
#include "rpgskill.h"
#include "rpgitem.h"
#include "rpgweapon.h"
#include "rpgarmor.h"
#include "rpgenemy.h"
#include "rpgtroop.h"
#include "rpgstate.h"
#include "rpganimation.h"
#include "rpgtileset.h"
#include "rpgcommonevent.h"

class DataSource;


class RPGSystem: public QObject
{
    Q_OBJECT
public:
    explicit RPGSystem(QObject *parent = nullptr);


    //Projekt stuff, not included in system file
    RPGString current_project_dir;
    RPGString graphics_dir;
    RPGString tileset_dir;
    RPGString characters_dir;
    RPGString battlers_dir;
    RPGString data_dir;
    RPGString autotiles_dir;
    RPGString audio_dir;

    QHash<int,RPGTileset*> tileset_hash;
    QList<RPGTileset*> tileset_list;

    QList<RPGMapInfo*> map_info_list; //map infos same order as in MapInfo.rxdata


    QList<RPGActor*> actor_list;
    QList<RPGClass*> classes_list;
    QList<RPGSkill*> skills_list;
    QList<RPGItem*> items_list;
    QList<RPGWeapon*> weapons_list;
    QList<RPGArmor*> armors_list;
    QList<RPGEnemy*> enemies_list;
    QList<RPGTroop*> troops_list;
    QList<RPGState*> states_list;
    QList<RPGAnimation*> animation_list;
    //tilesets see above
    QList<RPGCommonEvent*> common_events_list;


    enum {ACTORS, CLASSES, SKILLS, ITEMS, WEAPONS, ARMORS, ENEMIES, TROOPS, STATES, ANIMATIONS, TILESETS, COMMONEVENTS, SYSTEM};

    QColor purple = QColor(0x8a,0x16,0xa0);

    int current_map_id = -1;
    RPGMapInfo* get_current_map_info() {
        if(current_map_id == -1) return 0;
        else return this->map_info_list.at(current_map_id);}








    //System file
    QList<RPGString> variable_names;
    RPGAudioFile cancel_se;
    int magic_number = 0;
    RPGAudioFile escape_se;
    RPGAudioFile battle_end_me;
    int start_map_id = 0;
    RPGAudioFile shop_se;
    RPGString gameover_name;

    //QHash<RPGString, RPGString> words; //no idea for what we need this
    QList< QList<RPGString> > words;

    QList<RPGString> switches_names;
    RPGAudioFile decision_se;
    int edit_map_id = 0;
    RPGAudioFile battle_start_se;
    RPGAudioFile battle_bgm;
    int test_troop_id = 0;
    RPGAudioFile equip_se;
    RPGString title_name;
    RPGAudioFile enemy_collapse_se;
    RPGAudioFile cursor_se;
    QList<RPGString> elements;
    int start_y = 0;
    int battler_hue = 0;
    RPGAudioFile load_se;
    RPGAudioFile title_bgm;
    RPGAudioFile buzzer_se;
    RPGString windowskin_name;

    //QHash <RPGString, int> test_battlers; //not relevant for pokemon, currently withdrawn. TODO, wrong datatype. should be a list of qhash
    QList< QList<QVariantList> > test_battlers;

    int _ = 0; // ???
    RPGString battleback_name;
    QList<int> party_members;
    RPGAudioFile actor_collapse_se;
    RPGAudioFile gameover_me;
    RPGString battler_name;
    RPGAudioFile save_se;
    RPGString battle_transition;
    int start_x = 0;



    QStringList param_order;
signals:

};

#endif // RPGSYSTEM_H
