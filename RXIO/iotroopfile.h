#ifndef IOTROOPFILE_H
#define IOTROOPFILE_H

#include <QObject>
#include "fileparser.h"
#include "RXObjects/rpgtroop.h"

class IOTroopFile : public FileParser
{
    Q_OBJECT
public:
    IOTroopFile();
    IOTroopFile(QString path, QList<RPGTroop*> *troop_list);

    void write_to_file(QString path, QList<RPGTroop*> *troop_list);

};

#endif // IOTROOPFILE_H
