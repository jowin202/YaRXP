#ifndef RPGSYSTEM_H
#define RPGSYSTEM_H

#include <QObject>
#include "rpgaudiofile.h"
#include "rpgtileset.h"
#include "rpgstring.h"
#include "rpgmapinfo.h"


class RPGSystem: public QObject
{
    Q_OBJECT
public:
    explicit RPGSystem(QObject *parent = nullptr);



    RPGString current_project_dir;
    RPGString grahpics_dir;
    RPGString tileset_dir;
    RPGString characters_dir;
    RPGString data_dir;
    RPGString autotiles_dir;

    QHash<int,RPGTileset*> tileset_hash;
    QList<RPGTileset*> tileset_list;  //TODO Remove one of these

    //QHash<QString,Autotileset*> autotilesets;
    QList<RPGMapInfo*> map_info_list; //map infos same order as in MapInfo.rxdata


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
