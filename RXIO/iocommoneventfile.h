#ifndef IOCOMMONEVENTFILE_H
#define IOCOMMONEVENTFILE_H

#include <QObject>
#include "fileparser.h"
#include "RXObjects/rpgcommonevent.h"

class IOCommonEventFile : public FileParser
{
    Q_OBJECT
public:
    IOCommonEventFile();
    IOCommonEventFile(QString path, QList<RPGCommonEvent*> *common_event_list);

    void write_to_file(QString path, QList<RPGCommonEvent*> *common_event_list);
};

#endif // IOCOMMONEVENTFILE_H
