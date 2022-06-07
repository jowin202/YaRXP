#ifndef RXDATAWRITER_H
#define RXDATAWRITER_H

#include <QVariant>
#include <QObject>

class RXDataWriter : public QObject
{
    Q_OBJECT
public:
    explicit RXDataWriter(QString file = "");

    void write_symbol_or_link(QString symbol);
    void write_string(QString string);
    void write_variant(QVariant variant);
    void read_integer(int val);
    void read_bool(bool val);
    void write_fixnum(int fixnum);



signals:


private:
    QStringList symbol_cache;
};

#endif // RXDATAWRITER_H
