#ifndef RXDATAPARSER_H
#define RXDATAPARSER_H

#include <QObject>
#include <QFile>
#include <QRegExp>
#include <QHash>
#include <QVariant>

#include "rpgmapinfo.h"
#include "rpgmap.h"
#include <QDebug>

class RXDataParser : public QObject
{
    Q_OBJECT
public:
    explicit RXDataParser(QString file = "");
    void check_header();
    void close_file_if_open();
    int get_byte_and_rev();
    int read_fixnum();
    int read_one_byte();


    QString read_symbol_or_link();
    QString read_string();
    QVariant read_variant();
    int read_integer();
    bool read_bool();


    RPGMapInfo* read_mapinfo_object();
    RPGAudioFile* read_audiofile_object();
    void read_event_list(QList<RPGEvent*> *list);
    RPGEvent *read_event_object();
    void read_event_pages_list(QList<RPGEventPage*> *list);
    RPGEventPage *read_event_page_object();
    void read_event_command_list(QList<RPGEventCommand*> *list);
    RPGEventCommand *read_event_command_object();
    RPGEventPageCondition *read_event_page_condition_object();
    RPGMoveRoute *read_move_route_object();
    RPGMoveCommand *read_move_command_object();
    void read_move_command_list(QList<RPGMoveCommand*> *list);
    RPGEventPageGraphic *read_event_page_graphic();

    void parseMapInfo(QList<RPGMapInfo *> *map_list);
    RPGMap *parseMap();


signals:

private:
    QString filePath;
    QFile file;
    QStringList symbol_cache;
    bool test = false;

};

#endif // RXDATAPARSER_H
