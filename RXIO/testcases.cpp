#include "iomapinfofile.h"
#include "iotilesetfile.h"
#include "iosystemfile.h"
#include "ioactorfile.h"
#include "ioclassfile.h"
#include "iomapfile.h"

#include "iorgssad.h"
#include "RXObjects/parserexception.h"

#include "testcases.h"

#include <QTemporaryFile>

Testcases::Testcases(QObject *parent) : QObject(parent)
{

}

Testcases::Testcases(RPGSettings *settings)
{
    this->ok = true;
    QByteArray hash1, hash2;

    int num_maps = 0;
    int passed = 0;

    for (int i = 0; i <= 999; i++)
    {
        QString filler;
        if (i >= 0 && i <= 9) filler = "00";
        else if (i >= 10 && i <= 99) filler = "0";
        else filler = "";

        RPGMap *map = new RPGMap();
        QString path = settings->data_dir + "Map" + filler + QString::number(i) + ".rxdata";
        if (QFile(path).exists())
        {
            num_maps++;
            IOMapFile mapfile(path, map);
            hash1 = mapfile.getHash();

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


    QList<RPGMapInfo*> mapinfo_list;
    IOMapInfoFile info_file(settings->data_dir + "MapInfos.rxdata", &mapinfo_list);
    hash1 = info_file.getHash();

    QTemporaryFile tmpfile;
    tmpfile.setAutoRemove(true);
    tmpfile.open();
    tmpfile.close();
    info_file.write_to_file(tmpfile.fileName(), &mapinfo_list);
    hash2 = info_file.getHash();

    qDebug() << "MapInfos..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
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




    //Check system
    RPGSettings system;
    IOSystemFile system_file(settings->data_dir + "System.rxdata", &system);
    hash1 = system_file.getHash();

    system_file.write_to_file(tmpfile.fileName(), &system);
    hash2 = system_file.getHash();

    qDebug() << "System..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;



    //Check Actors
    QList<RPGActor*> actor_list;
    IOActorFile actor_file(settings->data_dir + "Actors.rxdata", &actor_list);
    hash1 = actor_file.getHash();


    actor_file.write_to_file(tmpfile.fileName(), &actor_list);
    hash2 = actor_file.getHash();


    qDebug() << "Actors..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;





    //Check Classes
    QList<RPGClass*> class_list;
    IOClassFile class_file(settings->data_dir + "Classes.rxdata", &class_list);
    hash1 = class_file.getHash();


    class_file.write_to_file(tmpfile.fileName(), &class_list);
    hash2 = class_file.getHash();


    qDebug() << "Classes..." << (hash1==hash2 ? "passed":"failed");
    if (hash1!=hash2)
        ok = false;


}
