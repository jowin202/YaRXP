#include "mainwindow.h"
#include <QDebug>
#include <QApplication>

#include "editors/dataeditor.h"

#include "RXIO/iotilesetfile.h"


#include <QList>

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("jowin202");
    QCoreApplication::setOrganizationDomain("github.com/jowin202");
    QCoreApplication::setApplicationName("YaRXP");

    MainWindow w;
    w.showMaximized();



    //w.open_project("/home/johannes/RPG_maker/pokemon_decrypted/Game.rxproj");
    //w.on_actionRun_Testcases_triggered();


    return a.exec();

}
