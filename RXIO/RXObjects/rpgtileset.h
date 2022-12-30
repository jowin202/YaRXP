#ifndef RPGTILESET_H
#define RPGTILESET_H

#include <QImage>
#include <QObject>
#include <QFile>
#include <QMessageBox>

#include "RXIO2/autotileset.h"
#include "rpgstring.h"


class RPGSystem;

class RPGTileset : public QObject
{
    Q_OBJECT
public:
    explicit RPGTileset(QObject *parent = nullptr);

    QImage getTile(int tile);
    QImage getAutoTile(int autotileset, int tile);

   void load_tileset_graphic(RPGSystem *system);




    int id;
    int panorama_hue;
    QList<int> terrain_tag;
    QList<int> priorities;
    QList<int> passages;
    RPGString name;
    RPGString panorama_name;
    RPGString fog_name;
    int fog_sx;
    int fog_sy;
    int fog_hue;
    int fog_opacity;
    int fog_zoom;
    int fog_blend_type;

    QList<RPGString> autotile_names;
    RPGString tileset_name;
    RPGString battleback_name;

    QImage tileset;
    QList<Autotileset> autotiles;


    QStringList param_order;
signals:

};

#endif // RPGTILESET_H
