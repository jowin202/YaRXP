#ifndef IOARMORFILE_H
#define IOARMORFILE_H

#include <QObject>
#include "fileparser.h"
#include "RXObjects/rpgarmor.h"

class IOArmorFile : public FileParser
{
    Q_OBJECT
public:
    IOArmorFile();
    IOArmorFile(QString path, QList<RPGArmor*> *armor_list);

    void write_to_file(QString path, QList<RPGArmor*> *armor_list);
};

#endif // IOARMORFILE_H
