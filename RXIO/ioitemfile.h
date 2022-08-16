#ifndef IOITEMFILE_H
#define IOITEMFILE_H

#include <QObject>
#include "RXObjects/rpgitem.h"
#include "fileparser.h"

class IOItemFile : public FileParser
{
    Q_OBJECT
public:
    IOItemFile();
    IOItemFile(QString path, QList<RPGItem*> *item_list);


    void write_to_file(QString path, QList<RPGItem*> *item_list);
};

#endif // IOITEMFILE_H
