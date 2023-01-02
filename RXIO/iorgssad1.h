#ifndef IORGSSAD1_H
#define IORGSSAD1_H

#include <QObject>
#include <QDir>
#include <QDebug>
#include "QFile"
#include "fileparser.h"

class IORGSSAD : public FileParser
{
    Q_OBJECT
public:
    explicit IORGSSAD(QObject *parent = nullptr);
    IORGSSAD(QString path, QString output_dir);

    int read_and_decrypt_integer();
    QString read_and_decrypt_string(int length);


signals:


private:
    quint64 key = 0xDEADCAFE;

};

#endif // IORGSSAD1_H
