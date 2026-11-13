#ifndef WRITER_H
#define WRITER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

#include <QSaveFile>
#include <QHash>

#include <QDebug>

class Writer : public QObject
{
    Q_OBJECT
public:
    explicit Writer(QJsonDocument *doc, QString file_location, bool strings_from_base_64 = false, bool extended_dictionary = false);


    void write_token(QJsonValue token);



    void write_32_bit(int n);

    void write_symbol_or_link(QString str);
    void write_one_byte(int byte);
    void write_fixnum(int n);
    void write_integer(qint64 n);
    void write_bignum(qint64 n);
    void write_string(QString str);


signals:


private:
    QSaveFile f;
    QHash<QString,int> symbol_cache;
    QJsonDocument *doc;

    bool strings_from_base_64 = false;
    bool extended_dictionary = false;
};

#endif // WRITER_H
