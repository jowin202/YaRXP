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
    audiowidget.cpp \
    autotileset.cpp \
    event_dialogs/choicesdialog.cpp \
    event_dialogs/commands/showtext.cpp \
    event_dialogs/conditionalbranchdialog.cpp \
    event_dialogs/eventcommanddialog.cpp \
    event_dialogs/eventdialog.cpp \
    event_dialogs/eventlist.cpp \
    event_dialogs/eventpage.cpp \
    event_dialogs/moveroutedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    mappropertiesdialog.cpp \
    maptreewidget.cpp \
    mapwidget.cpp \
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
    parser/rpgtileset.cpp \
    parser/rpgtone.cpp \
    parser/rxdataparser.cpp \
    parser/rxdatawriter.cpp \
    settings.cpp \
    tilesetwidget.cpp

HEADERS += \
    audiowidget.h \
    autotileset.h \
    event_dialogs/choicesdialog.h \
    event_dialogs/commands/showtext.h \
    event_dialogs/conditionalbranchdialog.h \
    event_dialogs/eventcommanddialog.h \
    event_dialogs/eventdialog.h \
    event_dialogs/eventlist.h \
    event_dialogs/eventpage.h \
    event_dialogs/moveroutedialog.h \
    mainwindow.h \
    mappropertiesdialog.h \
    maptreewidget.h \
    mapwidget.h \
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
    parser/rpgtileset.h \
    parser/rpgtone.h \
    parser/rxdataparser.h \
    parser/rxdatawriter.h \
    settings.h \
    tilesetwidget.h

FORMS += \
    audiowidget.ui \
    event_dialogs/choicesdialog.ui \
    event_dialogs/conditionalbranchdialog.ui \
    event_dialogs/eventcommanddialog.ui \
    event_dialogs/eventdialog.ui \
    event_dialogs/eventpage.ui \
    event_dialogs/moveroutedialog.ui \
    mainwindow.ui \
    mappropertiesdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
