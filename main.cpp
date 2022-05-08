#include "mainwindow.h"
#include <QDebug>
#include <QApplication>

#include "eventdialog.h"

#include "parser/rxdataparser.h"
#include "parser/rpgmapinfo.h"

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("jowin202");
    QCoreApplication::setOrganizationDomain("github.com/jowin202");
    QCoreApplication::setApplicationName("YaRXP");


    /*
    QList<RPGMapInfo *> map_list;
    RXDataParser parser("/home/johannes/RPG_maker/Pokemon Klagenfurt Vibes/Data/MapInfos.rxdata");
    parser.parseMapInfo(&map_list);

    for (int i = 0; i < map_list.length(); i++)
        map_list.at(i)->debug();
    */


    //RXDataParser parser("/home/johannes/RPG_maker/Pokemon Klagenfurt Vibes/Data/Map077.rxdata");
    //parser.parseMap();

    //return 0;
    MainWindow w;
    w.show();
    return a.exec();

}
