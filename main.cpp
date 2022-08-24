#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include "RXIO/ioactorfile.h"

#include "editors/dataeditor.h"


#include <QList>

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("jowin202");
    QCoreApplication::setOrganizationDomain("github.com/jowin202");
    QCoreApplication::setApplicationName("YaRXP");


    /*
    QList<RPGActor*> actor_list;
    IOActorFile file("/home/johannes/RPG_maker/pokemon_decrypted/Data/Actors.rxdata", &actor_list);

    for (int i = 0; i < 20; i++)
        qDebug() << actor_list.at(0)->parameters.at(4+i);


    DataEditor da;
    da.set_widget(DataEditor::ACTORS);
    da.show();
    */

    MainWindow w;
    w.show();


    return a.exec();

}
