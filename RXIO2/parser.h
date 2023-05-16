#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

#include <QFile>

#include <QDebug>

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QJsonDocument *doc, QString file_location, bool strings_to_base_64 = false, bool extended_dictionary = false);

    QJsonValue parse_token();

    int read_one_byte();
    int read_fixnum();
    int read_16_bit();
    int read_16_bit_signed();
    int read_32_bit();
    QString read_symbol_or_link(bool save_symbol = true);



signals:

private:
    QFile f;
    QStringList symbol_cache;
    QJsonDocument *doc;

    QMap<int,QJsonValue> reference_table;
    int object_count = -1;

    bool strings_to_base_64 = false;
    bool extended_dictionary = false;
};

#endif // PARSER_H
