#ifndef RXDATAPARSER_H
#define RXDATAPARSER_H

#include <QObject>
#include <QFile>
#include <QRegExp>
#include <QHash>
#include <QVariant>
#include <QColor>
#include <QDataStream>

#include "rpgmapinfo.h"
#include "rpgmap.h"
#include "rpgtileset.h"

#include "../settings.h"
#include <QDebug>

class RXDataParser : public QObject
{
    Q_OBJECT
public:
    explicit RXDataParser(QString file = "");
    void print_file_pos_as_hex();

    void check_header();
    void close_file_if_open();
    int get_byte_and_rev();
    int read_fixnum();
    int read_one_byte();
    int look_one_byte_ahead();
    QString look_ahead_object_type();



    QString read_symbol_or_link(bool save_symbol=true);
    QString read_string();
    QVariant read_variant();
    int read_integer();
    bool read_bool();


    RPGMapInfo* read_mapinfo_object();
    void read_audiofile_object(RPGAudioFile *audiofile);
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



    RPGTileset *parseTileset();
    void parseTilesetList(QHash<int, RPGTileset *> *tileset_hash, QList<RPGTileset *> *tileset_list);


    void parseSystem(Settings *settings);



signals:

private:
    QString filePath;
    QFile file;
    QStringList symbol_cache;


};

#endif // RXDATAPARSER_H
