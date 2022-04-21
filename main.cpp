#include "parser/rxdataparser.h"
#include "mainwindow.h"
#include <QDebug>

#include <QApplication>

int main(int argc, char *argv[])
{
    //QString file = "/home/johannes/RPG_maker/Pokemon Klagenfurt Vibes/Data/MapInfos.rxdata";
    //RXDataParser parser(file);

    //parser.parseMapInfo();


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
