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

    void create_dir_structure();
    void write_to_file(int id, QString name);


    QString random_id();


private:
    RPGDB *db;
    QString path;
    QMap <int,QString> map_id_to_dir;


    int counter = 0;

};

#endif // GODOTEXPORTER_H
