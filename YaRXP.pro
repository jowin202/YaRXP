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
    RXIO/RXObjects/autotileset.cpp \
    RXIO/RXObjects/parserexception.cpp \
    RXIO/RXObjects/rpgactor.cpp \
    RXIO/RXObjects/rpganimation.cpp \
    RXIO/RXObjects/rpganimationframe.cpp \
    RXIO/RXObjects/rpganimationtiming.cpp \
    RXIO/RXObjects/rpgarmor.cpp \
    RXIO/RXObjects/rpgaudiofile.cpp \
    RXIO/RXObjects/rpgclass.cpp \
    RXIO/RXObjects/rpgcommonevent.cpp \
    RXIO/RXObjects/rpgenemy.cpp \
    RXIO/RXObjects/rpgenemyaction.cpp \
    RXIO/RXObjects/rpgevent.cpp \
    RXIO/RXObjects/rpgeventcommand.cpp \
    RXIO/RXObjects/rpgeventpage.cpp \
    RXIO/RXObjects/rpgitem.cpp \
    RXIO/RXObjects/rpgmap.cpp \
    RXIO/RXObjects/rpgmapinfo.cpp \
    RXIO/RXObjects/rpgmovecommand.cpp \
    RXIO/RXObjects/rpgmoveroute.cpp \
    RXIO/RXObjects/rpgskill.cpp \
    RXIO/RXObjects/rpgstate.cpp \
    RXIO/RXObjects/rpgstring.cpp \
    RXIO/RXObjects/rpgsystem.cpp \
    RXIO/RXObjects/rpgtileset.cpp \
    RXIO/RXObjects/rpgtone.cpp \
    RXIO/RXObjects/rpgtroop.cpp \
    RXIO/RXObjects/rpgtroopmember.cpp \
    RXIO/RXObjects/rpgtrooppage.cpp \
    RXIO/RXObjects/rpgvariant.cpp \
    RXIO/RXObjects/rpgweapon.cpp \
    RXIO/fileparser.cpp \
    RXIO/ioactorfile.cpp \
    RXIO/ioanimationfile.cpp \
    RXIO/ioarmorfile.cpp \
    RXIO/ioclassfile.cpp \
    RXIO/iocommoneventfile.cpp \
    RXIO/ioenemyfile.cpp \
    RXIO/ioitemfile.cpp \
    RXIO/iomapfile.cpp \
    RXIO/iomapinfofile.cpp \
    RXIO/iorgssad.cpp \
    RXIO/ioscriptfile.cpp \
    RXIO/ioskillfile.cpp \
    RXIO/iostatefile.cpp \
    RXIO/iosystemfile.cpp \
    RXIO/iotilesetfile.cpp \
    RXIO/iotroopfile.cpp \
    RXIO/ioweaponfile.cpp \
    RXIO/testcases.cpp \
    audiowidget.cpp \
    datasource.cpp \
    editors/actors/editactors.cpp \
    editors/actors/editexpcurve.cpp \
    editors/actors/editoractorparameterlabel.cpp \
    editors/actors/editoractorparameterpage.cpp \
    editors/actors/editoractorparameters.cpp \
    editors/classes/editclasses.cpp \
    editors/dataeditor.cpp \
    event_dialogs/choicesdialog.cpp \
    event_dialogs/commands/multilineeventcell.cpp \
    event_dialogs/commands/showtext.cpp \
    event_dialogs/commands/simpleeventcell.cpp \
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
    RXIO/RXObjects/autotileset.h \
    RXIO/RXObjects/parserexception.h \
    RXIO/RXObjects/rpgactor.h \
    RXIO/RXObjects/rpganimation.h \
    RXIO/RXObjects/rpganimationframe.h \
    RXIO/RXObjects/rpganimationtiming.h \
    RXIO/RXObjects/rpgarmor.h \
    RXIO/RXObjects/rpgaudiofile.h \
    RXIO/RXObjects/rpgclass.h \
    RXIO/RXObjects/rpgcommonevent.h \
    RXIO/RXObjects/rpgenemy.h \
    RXIO/RXObjects/rpgenemyaction.h \
    RXIO/RXObjects/rpgevent.h \
    RXIO/RXObjects/rpgeventcommand.h \
    RXIO/RXObjects/rpgeventpage.h \
    RXIO/RXObjects/rpgitem.h \
    RXIO/RXObjects/rpgmap.h \
    RXIO/RXObjects/rpgmapinfo.h \
    RXIO/RXObjects/rpgmovecommand.h \
    RXIO/RXObjects/rpgmoveroute.h \
    RXIO/RXObjects/rpgskill.h \
    RXIO/RXObjects/rpgstate.h \
    RXIO/RXObjects/rpgstring.h \
    RXIO/RXObjects/rpgsystem.h \
    RXIO/RXObjects/rpgtileset.h \
    RXIO/RXObjects/rpgtone.h \
    RXIO/RXObjects/rpgtroop.h \
    RXIO/RXObjects/rpgtroopmember.h \
    RXIO/RXObjects/rpgtrooppage.h \
    RXIO/RXObjects/rpgvariant.h \
    RXIO/RXObjects/rpgweapon.h \
    RXIO/fileparser.h \
    RXIO/ioactorfile.h \
    RXIO/ioanimationfile.h \
    RXIO/ioarmorfile.h \
    RXIO/ioclassfile.h \
    RXIO/iocommoneventfile.h \
    RXIO/ioenemyfile.h \
    RXIO/ioitemfile.h \
    RXIO/iomapfile.h \
    RXIO/iomapinfofile.h \
    RXIO/iorgssad.h \
    RXIO/ioscriptfile.h \
    RXIO/ioskillfile.h \
    RXIO/iostatefile.h \
    RXIO/iosystemfile.h \
    RXIO/iotilesetfile.h \
    RXIO/iotroopfile.h \
    RXIO/ioweaponfile.h \
    RXIO/testcases.h \
    audiowidget.h \
    datasource.h \
    editors/actors/editactors.h \
    editors/actors/editexpcurve.h \
    editors/actors/editoractorparameterlabel.h \
    editors/actors/editoractorparameterpage.h \
    editors/actors/editoractorparameters.h \
    editors/classes/editclasses.h \
    editors/dataeditor.h \
    event_dialogs/choicesdialog.h \
    event_dialogs/commands/multilineeventcell.h \
    event_dialogs/commands/showtext.h \
    event_dialogs/commands/simpleeventcell.h \
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
    editors/actors/editactors.ui \
    editors/actors/editexpcurve.ui \
    editors/actors/editoractorparameterpage.ui \
    editors/actors/editoractorparameters.ui \
    editors/classes/editclasses.ui \
    editors/dataeditor.ui \
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
