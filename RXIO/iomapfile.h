#ifndef IOMAPFILE_H
#define IOMAPFILE_H

#include <QObject>

#include "RXObjects/rpgmap.h"
#include "fileparser.h"

class IOMapFile : public FileParser
{
    Q_OBJECT
public:
    explicit IOMapFile(QObject *parent = nullptr);

    IOMapFile(QString path, RPGMap *map);

    void write_to_file(QString path);

signals:

};

#endif // IOMAPFILE_H
