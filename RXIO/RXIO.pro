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
    RXObjects/parserexception.cpp \
    RXObjects/rpgaudiofile.cpp \
    RXObjects/rpgevent.cpp \
    RXObjects/rpgeventcommand.cpp \
    RXObjects/rpgeventpage.cpp \
    RXObjects/rpgmap.cpp \
    RXObjects/rpgmapinfo.cpp \
    RXObjects/rpgmovecommand.cpp \
    RXObjects/rpgmoveroute.cpp \
    RXObjects/rpgsettings.cpp \
    RXObjects/rpgstring.cpp \
    RXObjects/rpgtileset.cpp \
    RXObjects/rpgtone.cpp \
    fileparser.cpp \
    iomapfile.cpp \
    iomapinfofile.cpp \
    iosystemfile.cpp \
    iotilesetfile.cpp \
    main.cpp

HEADERS += \
    RXObjects/parserexception.h \
    RXObjects/rpgaudiofile.h \
    RXObjects/rpgevent.h \
    RXObjects/rpgeventcommand.h \
    RXObjects/rpgeventpage.h \
    RXObjects/rpgmap.h \
    RXObjects/rpgmapinfo.h \
    RXObjects/rpgmovecommand.h \
    RXObjects/rpgmoveroute.h \
    RXObjects/rpgsettings.h \
    RXObjects/rpgstring.h \
    RXObjects/rpgtileset.h \
    RXObjects/rpgtone.h \
    fileparser.h \
    iomapfile.h \
    iomapinfofile.h \
    iosystemfile.h \
    iotilesetfile.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
