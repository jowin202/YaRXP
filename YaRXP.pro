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
    RXIO/RXObjects/parserexception.cpp \
    RXIO/RXObjects/rpgactor.cpp \
    RXIO/RXObjects/rpgaudiofile.cpp \
    RXIO/RXObjects/rpgevent.cpp \
    RXIO/RXObjects/rpgeventcommand.cpp \
    RXIO/RXObjects/rpgeventpage.cpp \
    RXIO/RXObjects/rpgmap.cpp \
    RXIO/RXObjects/rpgmapinfo.cpp \
    RXIO/RXObjects/rpgmovecommand.cpp \
    RXIO/RXObjects/rpgmoveroute.cpp \
    RXIO/RXObjects/rpgsettings.cpp \
    RXIO/RXObjects/rpgstring.cpp \
    RXIO/RXObjects/rpgtileset.cpp \
    RXIO/RXObjects/rpgtone.cpp \
    RXIO/RXObjects/rpgvariant.cpp \
    RXIO/fileparser.cpp \
    RXIO/ioactorfile.cpp \
    RXIO/iomapfile.cpp \
    RXIO/iomapinfofile.cpp \
    RXIO/iorgssad.cpp \
    RXIO/iosystemfile.cpp \
    RXIO/iotilesetfile.cpp \
    RXIO/testcases.cpp \
    audiowidget.cpp \
    autotileset.cpp \
    editors/animationeditor.cpp \
    editors/commoneventseditor.cpp \
    editors/systemeditor.cpp \
    editors/tileseteditor.cpp \
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
    tilesetwidget.cpp

HEADERS += \
    RXIO/RXObjects/parserexception.h \
    RXIO/RXObjects/rpgactor.h \
    RXIO/RXObjects/rpgaudiofile.h \
    RXIO/RXObjects/rpgevent.h \
    RXIO/RXObjects/rpgeventcommand.h \
    RXIO/RXObjects/rpgeventpage.h \
    RXIO/RXObjects/rpgmap.h \
    RXIO/RXObjects/rpgmapinfo.h \
    RXIO/RXObjects/rpgmovecommand.h \
    RXIO/RXObjects/rpgmoveroute.h \
    RXIO/RXObjects/rpgsettings.h \
    RXIO/RXObjects/rpgstring.h \
    RXIO/RXObjects/rpgtileset.h \
    RXIO/RXObjects/rpgtone.h \
    RXIO/RXObjects/rpgvariant.h \
    RXIO/fileparser.h \
    RXIO/ioactorfile.h \
    RXIO/iomapfile.h \
    RXIO/iomapinfofile.h \
    RXIO/iorgssad.h \
    RXIO/iosystemfile.h \
    RXIO/iotilesetfile.h \
    RXIO/testcases.h \
    audiowidget.h \
    autotileset.h \
    editors/animationeditor.h \
    editors/commoneventseditor.h \
    editors/systemeditor.h \
    editors/tileseteditor.h \
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
    tilesetwidget.h

FORMS += \
    audiowidget.ui \
    editors/animationeditor.ui \
    editors/commoneventseditor.ui \
    editors/systemeditor.ui \
    editors/tileseteditor.ui \
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

SUBDIRS += \
    RXIO/RXIO.pro
