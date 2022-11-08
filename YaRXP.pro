QT       += core gui multimedia

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
    datasource.cpp \
    dialogs/audiodialog.cpp \
    dialogs/imagedialog.cpp \
    dialogs/imagedisplaywidget.cpp \
    dialogs/listdialog.cpp \
    dialogs/mappropertiesdialog.cpp \
    dialogs/objectcombodialog.cpp \
    dialogs/switchvariablewidget.cpp \
    editors/abcbox.cpp \
    editors/abclist.cpp \
    editors/actors/editactors.cpp \
    editors/actors/editexpcurve.cpp \
    editors/actors/editoractorparameterlabel.cpp \
    editors/actors/editoractorparameterpage.cpp \
    editors/actors/editoractorparameters.cpp \
    editors/actors/generatecurvedialog.cpp \
    editors/animations/editanimations.cpp \
    editors/armors/editarmors.cpp \
    editors/checkboxlist.cpp \
    editors/classes/editclasses.cpp \
    editors/classes/skilllearning.cpp \
    editors/commonevents/editcommonevents.cpp \
    editors/dataeditor.cpp \
    editors/enemies/editenemies.cpp \
    editors/enemies/enemyactiondialog.cpp \
    editors/enemies/treasuredialog.cpp \
    editors/items/edititems.cpp \
    editors/plusminusbox.cpp \
    editors/plusminuslist.cpp \
    editors/skills/editskills.cpp \
    editors/states/editstates.cpp \
    editors/tilesets/edittilesets.cpp \
    editors/troops/edittroops.cpp \
    editors/troops/troopeventpage.cpp \
    editors/troops/trooppageconditiondialog.cpp \
    editors/troops/trooppiclabel.cpp \
    editors/weapons/editweapons.cpp \
    event_dialogs/choicesdialog.cpp \
    event_dialogs/commands/multilineeventcell.cpp \
    event_dialogs/commands/simpleeventcell.cpp \
    event_dialogs/conditionalbranchdialog.cpp \
    event_dialogs/eventcommanddialog.cpp \
    event_dialogs/eventdialog.cpp \
    event_dialogs/eventlist.cpp \
    event_dialogs/eventpage.cpp \
    event_dialogs/moveroutedialog.cpp \
    main.cpp \
    mainwindow.cpp \
    viewer/maprectangle.cpp \
    viewer/mapselectrectangle.cpp \
    viewer/maptile.cpp \
    maptreewidget.cpp \
    viewer/mapview.cpp \
    viewer/tilesetrectangle.cpp \
    viewer/tilesetview.cpp

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
    datasource.h \
    dialogs/audiodialog.h \
    dialogs/imagedialog.h \
    dialogs/imagedisplaywidget.h \
    dialogs/listdialog.h \
    dialogs/mappropertiesdialog.h \
    dialogs/objectcombodialog.h \
    dialogs/switchvariablewidget.h \
    editors/abcbox.h \
    editors/abclist.h \
    editors/actors/editactors.h \
    editors/actors/editexpcurve.h \
    editors/actors/editoractorparameterlabel.h \
    editors/actors/editoractorparameterpage.h \
    editors/actors/editoractorparameters.h \
    editors/actors/generatecurvedialog.h \
    editors/animations/editanimations.h \
    editors/armors/editarmors.h \
    editors/checkboxlist.h \
    editors/classes/editclasses.h \
    editors/classes/skilllearning.h \
    editors/commonevents/editcommonevents.h \
    editors/dataeditor.h \
    editors/enemies/editenemies.h \
    editors/enemies/enemyactiondialog.h \
    editors/enemies/treasuredialog.h \
    editors/items/edititems.h \
    editors/plusminusbox.h \
    editors/plusminuslist.h \
    editors/skills/editskills.h \
    editors/states/editstates.h \
    editors/tilesets/edittilesets.h \
    editors/troops/edittroops.h \
    editors/troops/troopeventpage.h \
    editors/troops/trooppageconditiondialog.h \
    editors/troops/trooppiclabel.h \
    editors/weapons/editweapons.h \
    event_dialogs/choicesdialog.h \
    event_dialogs/commands/multilineeventcell.h \
    event_dialogs/commands/simpleeventcell.h \
    event_dialogs/conditionalbranchdialog.h \
    event_dialogs/eventcommanddialog.h \
    event_dialogs/eventdialog.h \
    event_dialogs/eventlist.h \
    event_dialogs/eventpage.h \
    event_dialogs/moveroutedialog.h \
    mainwindow.h \
    viewer/maprectangle.h \
    viewer/mapselectrectangle.h \
    viewer/maptile.h \
    maptreewidget.h \
    viewer/mapview.h \
    viewer/tilesetrectangle.h \
    viewer/tilesetview.h

FORMS += \
    dialogs/audiodialog.ui \
    dialogs/imagedialog.ui \
    dialogs/listdialog.ui \
    dialogs/mappropertiesdialog.ui \
    dialogs/objectcombodialog.ui \
    dialogs/switchvariablewidget.ui \
    editors/abcbox.ui \
    editors/abclist.ui \
    editors/actors/editactors.ui \
    editors/actors/editexpcurve.ui \
    editors/actors/editoractorparameterpage.ui \
    editors/actors/editoractorparameters.ui \
    editors/actors/generatecurvedialog.ui \
    editors/animations/editanimations.ui \
    editors/armors/editarmors.ui \
    editors/checkboxlist.ui \
    editors/classes/editclasses.ui \
    editors/classes/skilllearning.ui \
    editors/commonevents/editcommonevents.ui \
    editors/dataeditor.ui \
    editors/enemies/editenemies.ui \
    editors/enemies/enemyactiondialog.ui \
    editors/enemies/treasuredialog.ui \
    editors/items/edititems.ui \
    editors/plusminusbox.ui \
    editors/plusminuslist.ui \
    editors/skills/editskills.ui \
    editors/states/editstates.ui \
    editors/tilesets/edittilesets.ui \
    editors/troops/edittroops.ui \
    editors/troops/troopeventpage.ui \
    editors/troops/trooppageconditiondialog.ui \
    editors/weapons/editweapons.ui \
    event_dialogs/choicesdialog.ui \
    event_dialogs/conditionalbranchdialog.ui \
    event_dialogs/eventcommanddialog.ui \
    event_dialogs/eventdialog.ui \
    event_dialogs/eventpage.ui \
    event_dialogs/moveroutedialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

SUBDIRS += \
    RXIO/RXIO.pro
