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
    RPGAudioFile cancel_se;
    int magic_number = 0;
    RPGAudioFile escape_se;
    RPGAudioFile battle_end_me;
    int start_map_id = 0;
    RPGAudioFile shop_se;
    QString gameover_name;
    QHash<QString, QString> words; //no idea for what we need this
    QStringList switches_names;
    RPGAudioFile decision_se;
    int edit_map_id = 0;
    RPGAudioFile battle_start_se;
    RPGAudioFile battle_bgm;
    int test_troop_id = 0;
    RPGAudioFile equip_se;
    QString title_name;
    RPGAudioFile enemy_collapse_se;
    RPGAudioFile cursor_se;
    QStringList elements;
    int start_y = 0;
    int battler_hue = 0;
    RPGAudioFile load_se;
    RPGAudioFile title_bgm;
    RPGAudioFile buzzer_se;
    QString windowskin_name;
    QHash <QString, int> test_battlers; //not relevant for pokemon, currently withdrawn. TODO, wrong datatype. should be a list of qhash
    int _ = 0; // ???
    QString battleback_name;
    QList<int> party_members;
    RPGAudioFile actor_collapse_se;
    RPGAudioFile gameover_me;
    QString battler_name;
    RPGAudioFile save_se;
    QString battle_transition;
    int start_x = 0;


signals:

};

#endif // SETTINGS_H
