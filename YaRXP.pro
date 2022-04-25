QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    eventdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    maptreewidget.cpp \
    maptreewidgetitem.cpp \
    mapwidget.cpp \
    eventpage.cpp \
    parser/rpgaudiofile.cpp \
    parser/rpgevent.cpp \
    parser/rpgeventcommand.cpp \
    parser/rpgeventpage.cpp \
    parser/rpgeventpagecondition.cpp \
    parser/rpgeventpagegraphic.cpp \
    parser/rpgmap.cpp \
    parser/rpgmapinfo.cpp \
    parser/rpgmovecommand.cpp \
    parser/rpgmoveroute.cpp \
    parser/rxdataparser.cpp \
    tilesetwidget.cpp

HEADERS += \
    eventdialog.h \
    mainwindow.h \
    maptreewidget.h \
    maptreewidgetitem.h \
    mapwidget.h \
    eventpage.h \
    parser/rpgaudiofile.h \
    parser/rpgevent.h \
    parser/rpgeventcommand.h \
    parser/rpgeventpage.h \
    parser/rpgeventpagecondition.h \
    parser/rpgeventpagegraphic.h \
    parser/rpgmap.h \
    parser/rpgmapinfo.h \
    parser/rpgmovecommand.h \
    parser/rpgmoveroute.h \
    parser/rxdataparser.h \
    tilesetwidget.h

FORMS += \
    eventdialog.ui \
    mainwindow.ui \
    eventpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
