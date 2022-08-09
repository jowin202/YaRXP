#ifndef IOMAPINFOFILE_H
#define IOMAPINFOFILE_H

#include <QDebug>
#include <QObject>
#include <QFile>


#include "RXObjects/rpgmapinfo.h"
#include "fileparser.h"

class IOMapInfoFile : public FileParser
{
    Q_OBJECT
public:
    explicit IOMapInfoFile(QObject *parent = nullptr);
    IOMapInfoFile(QString path, QList<RPGMapInfo*> *mapinfo_list);

    void write_to_file(QString path, QList<RPGMapInfo*> *mapinfo_list);



signals:


private:

};

#endif // IOMAPINFOFILE_H
