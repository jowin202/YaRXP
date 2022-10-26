#ifndef IOTILESETFILE_H
#define IOTILESETFILE_H

#include <QObject>

#include "fileparser.h"
#include "RXObjects/rpgtileset.h"

class IOTilesetFile : public FileParser
{
    Q_OBJECT
public:
    explicit IOTilesetFile(QObject *parent = nullptr);
    IOTilesetFile(QString path, QHash<int,RPGTileset *> *tileset_hash, QList<RPGTileset*> *tileset_list);


    void write_to_file(QString path, QList<RPGTileset*> *tileset_list);



signals:

};

#endif // IOTILESETFILE_H
