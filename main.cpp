#include "mainwindow.h"
#include <QDebug>
#include <QApplication>

#include "event_dialogs/eventdialog.h"

#include "RXIO/RXObjects/rpgmapinfo.h"
#include "RXIO/RXObjects/rpgtileset.h"
#include "autotileset.h"



#include <QList>

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("jowin202");
    QCoreApplication::setOrganizationDomain("github.com/jowin202");
    QCoreApplication::setApplicationName("YaRXP");




    MainWindow w;
    w.show();
    return a.exec();

}
