#include "mainwindow.h"
#include <QDebug>
#include <QApplication>


#include <QList>

int main(int argc, char *argv[])
{    
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("jowin202");
    QCoreApplication::setOrganizationDomain("github.com/jowin202");
    QCoreApplication::setApplicationName("YaRXP");

    MainWindow w;
    w.showMaximized();

    return a.exec();

}
