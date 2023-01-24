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
    events/commands/changeactornamedialog.cpp \
    events/commands/changeequipmentdialog.cpp \
    events/commands/changemapsettingsdialog.cpp \
    events/commands/choicesdialog.cpp \
    events/commands/changecolortonedialog.cpp \
    events/commands/changestatedialog.cpp \
    events/commands/changetextoptionsdialog.cpp \
    events/commands/combocombodialog.cpp \
    events/commands/combospindialog.cpp \
    events/commands/dealdamagedialog.cpp \
    events/commands/forceactiondialog.cpp \
    events/commands/increasedecreasedialog.cpp \
    events/commands/picturedialog.cpp \
    events/commands/radiodialog.cpp \
    events/commands/screenshakedialog.cpp \
    events/commands/scrollmapdialog.cpp \
    events/commands/selfswitchdialog.cpp \
    events/commands/shopgoodsdialog.cpp \
    events/commands/shopprocessingdialog.cpp \
    events/commands/showbattleanimationdialog.cpp \
    events/commands/showtextdialog.cpp \
    events/commands/singlecombodialog.cpp \
    events/commands/spinspindialog.cpp \
    events/commands/timerdialog.cpp \
    events/commands/weatherdialog.cpp \
    events/commands/conditionalbranchdialog.cpp \
    events/eventcommanddialog.cpp \
    events/eventdialog.cpp \
    events/eventlistitem.cpp \
    events/eventpage.cpp \
    events/commands/moveroutedialog.cpp \
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
    events/commands/changeactornamedialog.h \
    events/commands/changeequipmentdialog.h \
    events/commands/changemapsettingsdialog.h \
    events/commands/choicesdialog.h \
    events/commands/changecolortonedialog.h \
    events/commands/changestatedialog.h \
    events/commands/changetextoptionsdialog.h \
    events/commands/combocombodialog.h \
    events/commands/combospindialog.h \
    events/commands/dealdamagedialog.h \
    events/commands/forceactiondialog.h \
    events/commands/increasedecreasedialog.h \
    events/commands/picturedialog.h \
    events/commands/radiodialog.h \
    events/commands/screenshakedialog.h \
    events/commands/scrollmapdialog.h \
    events/commands/selfswitchdialog.h \
    events/commands/shopgoodsdialog.h \
    events/commands/shopprocessingdialog.h \
    events/commands/showbattleanimationdialog.h \
    events/commands/showtextdialog.h \
    events/commands/singlecombodialog.h \
    events/commands/spinspindialog.h \
    events/commands/timerdialog.h \
    events/commands/weatherdialog.h \
    events/commands/conditionalbranchdialog.h \
    events/eventcommanddialog.h \
    events/eventdialog.h \
    events/eventlistitem.h \
    events/eventpage.h \
    events/commands/moveroutedialog.h \
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
    events/commands/changeactornamedialog.ui \
    events/commands/changeequipmentdialog.ui \
    events/commands/changemapsettingsdialog.ui \
    events/commands/choicesdialog.ui \
    events/commands/changecolortonedialog.ui \
    events/commands/changestatedialog.ui \
    events/commands/changetextoptionsdialog.ui \
    events/commands/combocombodialog.ui \
    events/commands/combospindialog.ui \
    events/commands/dealdamagedialog.ui \
    events/commands/forceactiondialog.ui \
    events/commands/increasedecreasedialog.ui \
    events/commands/picturedialog.ui \
    events/commands/radiodialog.ui \
    events/commands/screenshakedialog.ui \
    events/commands/scrollmapdialog.ui \
    events/commands/selfswitchdialog.ui \
    events/commands/shopgoodsdialog.ui \
    events/commands/shopprocessingdialog.ui \
    events/commands/showbattleanimationdialog.ui \
    events/commands/showtextdialog.ui \
    events/commands/singlecombodialog.ui \
    events/commands/spinspindialog.ui \
    events/commands/timerdialog.ui \
    events/commands/weatherdialog.ui \
    events/commands/conditionalbranchdialog.ui \
    events/eventcommanddialog.ui \
    events/eventdialog.ui \
    events/eventpage.ui \
    events/commands/moveroutedialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

SUBDIRS += \
    RXIO/RXIO.pro
