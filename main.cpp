#include "mainwindow.h"
#include <QDebug>
#include <QApplication>

#include "eventdialog.h"

#include "parser/rxdataparser.h"
#include "parser/rpgmapinfo.h"
#include "parser/rpgtileset.h"
#include "autotileset.h"

#include <QList>

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("jowin202");
    QCoreApplication::setOrganizationDomain("github.com/jowin202");
    QCoreApplication::setApplicationName("YaRXP");


    //QImage img = QImage("/home/johannes/RPG_maker/Pokemon Klagenfurt Vibes/Graphics/Autotiles/Sand shore.png");
    //Autotileset at(img);


    /*
    QList<RPGMapInfo *> map_list;
    RXDataParser parser("/home/johannes/RPG_maker/Pokemon Klagenfurt Vibes/Data/MapInfos.rxdata");
    parser.parseMapInfo(&map_list);

    for (int i = 0; i < map_list.length(); i++)
        map_list.at(i)->debug();
    */

    /*
    RXDataParser parser("/home/johannes/RPG_maker/Pokemon Klagenfurt Vibes/Data/Map048.rxdata");
    parser.parseMap();
    qDebug() << "finished";

    return 0;*/


    /*
    QList<RPGTileset*> tileset_list;
    RXDataParser parser2("/home/johannes/RPG_maker/Pokemon Klagenfurt Vibes/Data/Tilesets.rxdata");
    parser2.parseTilesetList(&tileset_list);

    return 0;
    */

    MainWindow w;
    w.show();
    return a.exec();

}
