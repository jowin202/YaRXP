#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QFile>
#include <QObject>
#include <QWidget>
#include <QVariantList>
#include <QCryptographicHash>


#include "RXObjects/rpgstring.h"
#include "RXObjects/parserexception.h"
#include "RXObjects/rpgaudiofile.h"
#include "RXObjects/rpgmoveroute.h"

class FileParser : public QObject
{
    Q_OBJECT
public:
    explicit FileParser(QObject *parent = nullptr);

    void check_header();
    void write_header();
    ParserException *getException(QString message);

    int read_one_byte();
    void write_one_byte(int byte);
    int look_one_byte_ahead();
    QString look_ahead_object_type();

    int look_ahead_code();

    QString read_symbol_or_link(bool save_symbol=true);
    void write_symbol_or_link(QString str);


    int read_fixnum();
    void write_fixnum(int n);

    RPGString read_string();
    void write_string(RPGString str);


    QVariant read_variant();

    int read_integer();
    void write_integer(int n);

    bool read_bool();
    void write_bool(bool val);

    int read_list();
    void write_list(int length);

    int read_array();
    void write_array(int length);

    QVariantList read_object();
    void write_object(QString symbol, int params);


    void read_table(QList<int> *list);
    void read_table_for_map(QList<int> *list);
    void write_table(QList<int> *list);

    void read_audiofile_object(RPGAudioFile *audiofile);
    void write_audiofile_object(RPGAudioFile *audiofile);




    void read_move_route_object(RPGMoveRoute *move_route_object);






    void displayHash();
signals:

protected:
    QString path;
    QFile file;
    QString last_visited_function;
    QStringList symbol_cache;
};

#endif // FILEPARSER_H
