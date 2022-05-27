#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "parser/rpgtileset.h"
#include "autotileset.h"
#include "parser/rpgmapinfo.h"

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    QString current_project_dir;
    QString grahpics_dir;
    QString tileset_dir;
    QString characters_dir;
    QString data_dir;
    QString autotiles_dir;

    QHash<int,RPGTileset*> tileset_hash;
    QList<RPGTileset*> tileset_list;  //TODO Remove one of these

    QHash<QString,Autotileset*> autotilesets;
    QList<RPGMapInfo*> map_info_list; //map infos same order as in MapInfo.rxdata


    //System file
    QStringList variable_names;
    RPGAudioFile *cancel_se = 0;
    int magic_number = 0;
    RPGAudioFile *escape_se = 0;
    RPGAudioFile *battle_end_me = 0;
    int start_map_id = 0;
    RPGAudioFile *shop_se= 0;
    QString gameover_name;
    QHash<QString, QString> words; //no idea for what we need this
    QStringList switches_names;
    RPGAudioFile *decision_se = 0;
    int edit_map_id = 0;
    RPGAudioFile *battle_start_se = 0;
    RPGAudioFile *battle_bgm = 0;
    int test_troop_id = 0;
    RPGAudioFile *equip_se = 0;
    QString title_name;
    RPGAudioFile *enemy_collapse_se = 0;
    RPGAudioFile *cursor_se = 0;
    QStringList elements;
    int start_y = 0;
    int battler_hue = 0;
    RPGAudioFile *load_se = 0;
    RPGAudioFile *title_bgm = 0;
    RPGAudioFile *buzzer_se = 0;
    QString windowskin_name;
    QHash <QString, int> test_battlers; //not relevant for pokemon, currently withdrawn. TODO, wrong datatype. should be a list of qhash
    int _ = 0; // ???
    QString battleback_name;
    QList<int> party_members;
    RPGAudioFile *actor_collapse_se = 0;
    RPGAudioFile *gameover_me = 0;
    QString battler_name;
    RPGAudioFile *save_se = 0;
    QString battle_transition;
    int start_x = 0;


signals:

};

#endif // SETTINGS_H
