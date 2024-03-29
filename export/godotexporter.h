#ifndef GODOTEXPORTER_H
#define GODOTEXPORTER_H

#include <QString>
#include <QMap>
#include <QJsonObject>
#include <QCryptographicHash>

class RPGDB;

class GodotExporter
{
public:
    GodotExporter(RPGDB *db, QString path);


    int array_position(int x, int y, int layer, int width, int height)
    {return x + y * width + height * width * layer;}

    QString create_tileset(int tileset_id, QJsonObject tileset);
    QVariantList create_autotile(QString name);

    void create_dir_structure();
    void write_map_to_file(int id, QString name);

    QString random_id();
    QString pseudorandom_id(QString input);


private:
    RPGDB *db;
    QString path;
    QMap <int,QString> map_id_to_dir;

    QMap<QString,QString> imported_tilesets;
    QMap<QString,QVariantList> imported_autotiles;

    int counter = 0;

    int direction_convert[4] = {3, 0, 1, 2 };
};

#endif // GODOTEXPORTER_H
