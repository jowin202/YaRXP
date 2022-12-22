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
    explicit Parser(QJsonDocument *doc, QMap<QString, QStringList> *param_oders, QString file_location);

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
    QMap<QString, QStringList> *param_oders;
    QJsonDocument *doc;

    QMap<int,QJsonValue> reference_table;
    int object_count = -1;
};

#endif // PARSER_H
