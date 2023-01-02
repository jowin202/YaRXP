#ifndef IORGSSAD_H
#define IORGSSAD_H

#include <QObject>
#include <QDir>
#include <QDebug>
#include "QFile"

class IORGSSAD
{
public:
    explicit IORGSSAD(QObject *parent = nullptr);
    IORGSSAD(QString path, QString output_dir);

    int read_and_decrypt_integer();
    QString read_and_decrypt_string(int length);


    int read_one_byte();

signals:


private:
    quint64 key = 0xDEADCAFE;
    QFile file;

};

#endif // IORGSSAD_H
