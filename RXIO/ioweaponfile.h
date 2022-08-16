#ifndef IOWEAPONFILE_H
#define IOWEAPONFILE_H

#include <QObject>
#include "fileparser.h"
#include "RXObjects/rpgweapon.h"

class IOWeaponFile : public FileParser
{
    Q_OBJECT
public:
    IOWeaponFile();
    IOWeaponFile(QString path, QList<RPGWeapon*> *weapon_list);

    void write_to_file(QString path, QList<RPGWeapon*> *weapon_list);
};

#endif // IOWEAPONFILE_H
