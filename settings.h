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


signals:

};

#endif // SETTINGS_H
