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
    if (argc > 1)
    {
        w.open_project(QString(argv[1]));
    }
    w.showMaximized();


    return a.exec();

}
