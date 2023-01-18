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
    RXIO/ioscriptfile.cpp \
    RXIO/ioskillfile.cpp \
    RXIO/iostatefile.cpp \
    RXIO/iosystemfile.cpp \
    RXIO/iotilesetfile.cpp \
    RXIO/iotroopfile.cpp \
    RXIO/ioweaponfile.cpp \
    RXIO/testcases.cpp \
    RXIO2/autotileset.cpp \
    RXIO2/factory.cpp \
    RXIO2/fileopener.cpp \
    RXIO2/iorgssad.cpp \
    RXIO2/parser.cpp \
    RXIO2/rpgdb.cpp \
    RXIO2/rpgeditorcontroller.cpp \
    RXIO2/rpgeventlistcontroller.cpp \
    RXIO2/rpgmapcontroller.cpp \
    RXIO2/rpgmapinfocontroller.cpp \
    RXIO2/rxexception.cpp \
    RXIO2/writer.cpp \
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
    editors/animations/animationgraphicpreview.cpp \
    editors/animations/animationlabel.cpp \
    editors/animations/cellpropertiesdialog.cpp \
    editors/animations/editanimations.cpp \
    editors/animations/flashsedialog.cpp \
    editors/animations/timingtable.cpp \
    editors/animations/tweeningdialog.cpp \
    editors/armors/editarmors.cpp \
    editors/checkboxlist.cpp \
    editors/classes/editclasses.cpp \
    editors/classes/skilllearning.cpp \
    editors/classes/tableskilllearning.cpp \
    editors/commonevents/editcommonevents.cpp \
    editors/dataeditor.cpp \
    editors/enemies/editenemies.cpp \
    editors/enemies/enemyactiondialog.cpp \
    editors/enemies/enemyactiontable.cpp \
    editors/items/edititems.cpp \
    editors/plusminusbox.cpp \
    editors/plusminuslist.cpp \
    editors/skills/editskills.cpp \
    editors/states/editstates.cpp \
    editors/system/editsystem.cpp \
    editors/tilesets/edittilesets.cpp \
    editors/tilesets/tile.cpp \
    editors/tilesets/tileseteditwidget.cpp \
    editors/troops/edittroops.cpp \
    editors/troops/troopeventpage.cpp \
    editors/troops/trooppageconditiondialog.cpp \
    editors/troops/trooppiclabel.cpp \
    editors/weapons/editweapons.cpp \
    events/choicesdialog.cpp \
    events/commands/multilineeventcell.cpp \
    events/commands/simpleeventcell.cpp \
    events/conditionalbranchdialog.cpp \
    events/eventcommanddialog.cpp \
    events/eventdialog.cpp \
    events/eventlist.cpp \
    events/eventpage.cpp \
    events/moveroutedialog.cpp \
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
    RXIO/ioscriptfile.h \
    RXIO/ioskillfile.h \
    RXIO/iostatefile.h \
    RXIO/iosystemfile.h \
    RXIO/iotilesetfile.h \
    RXIO/iotroopfile.h \
    RXIO/ioweaponfile.h \
    RXIO/testcases.h \
    RXIO2/autotileset.h \
    RXIO2/factory.h \
    RXIO2/fileopener.h \
    RXIO2/iorgssad.h \
    RXIO2/parser.h \
    RXIO2/rpgdb.h \
    RXIO2/rpgeditorcontroller.h \
    RXIO2/rpgeventlistcontroller.h \
    RXIO2/rpgmapcontroller.h \
    RXIO2/rpgmapinfocontroller.h \
    RXIO2/rxexception.h \
    RXIO2/writer.h \
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
    editors/animations/animationgraphicpreview.h \
    editors/animations/animationlabel.h \
    editors/animations/cellpropertiesdialog.h \
    editors/animations/editanimations.h \
    editors/animations/flashsedialog.h \
    editors/animations/timingtable.h \
    editors/animations/tweeningdialog.h \
    editors/armors/editarmors.h \
    editors/checkboxlist.h \
    editors/classes/editclasses.h \
    editors/classes/skilllearning.h \
    editors/classes/tableskilllearning.h \
    editors/commonevents/editcommonevents.h \
    editors/dataeditor.h \
    editors/enemies/editenemies.h \
    editors/enemies/enemyactiondialog.h \
    editors/enemies/enemyactiontable.h \
    editors/items/edititems.h \
    editors/plusminusbox.h \
    editors/plusminuslist.h \
    editors/skills/editskills.h \
    editors/states/editstates.h \
    editors/system/editsystem.h \
    editors/tilesets/edittilesets.h \
    editors/tilesets/tile.h \
    editors/tilesets/tileseteditwidget.h \
    editors/troops/edittroops.h \
    editors/troops/troopeventpage.h \
    editors/troops/trooppageconditiondialog.h \
    editors/troops/trooppiclabel.h \
    editors/weapons/editweapons.h \
    events/choicesdialog.h \
    events/commands/multilineeventcell.h \
    events/commands/simpleeventcell.h \
    events/conditionalbranchdialog.h \
    events/eventcommanddialog.h \
    events/eventdialog.h \
    events/eventlist.h \
    events/eventpage.h \
    events/moveroutedialog.h \
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
    editors/animations/cellpropertiesdialog.ui \
    editors/animations/editanimations.ui \
    editors/animations/flashsedialog.ui \
    editors/animations/tweeningdialog.ui \
    editors/armors/editarmors.ui \
    editors/checkboxlist.ui \
    editors/classes/editclasses.ui \
    editors/classes/skilllearning.ui \
    editors/commonevents/editcommonevents.ui \
    editors/dataeditor.ui \
    editors/enemies/editenemies.ui \
    editors/enemies/enemyactiondialog.ui \
    editors/items/edititems.ui \
    editors/plusminusbox.ui \
    editors/plusminuslist.ui \
    editors/skills/editskills.ui \
    editors/states/editstates.ui \
    editors/system/editsystem.ui \
    editors/tilesets/edittilesets.ui \
    editors/troops/edittroops.ui \
    editors/troops/troopeventpage.ui \
    editors/troops/trooppageconditiondialog.ui \
    editors/weapons/editweapons.ui \
    events/choicesdialog.ui \
    events/conditionalbranchdialog.ui \
    events/eventcommanddialog.ui \
    events/eventdialog.ui \
    events/eventpage.ui \
    events/moveroutedialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

SUBDIRS += \
    RXIO/RXIO.pro
