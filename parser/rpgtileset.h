#ifndef RPGTILESET_H
#define RPGTILESET_H

#include <QImage>
#include <QObject>

#include "../autotileset.h"

class RPGTileset : public QObject
{
    Q_OBJECT
public:
    explicit RPGTileset(QObject *parent = nullptr);

    int id;
    int panorama_hue;
    QList<int> terrain_tag;
    QList<int> priorities;
    QList<int> passages;
    QString name;
    QString panorama_name;
    QString fog_name;
    int fog_sx;
    int fog_sy;
    int fog_hue;
    int fog_opacity;
    int fog_zoom;
    int fog_blend_type;

    QStringList autotile_names;
    QString tileset_name;
    QString battleback_name;

    QImage tileset;
    QList<Autotileset> autotiles;


signals:

};

#endif // RPGTILESET_H
