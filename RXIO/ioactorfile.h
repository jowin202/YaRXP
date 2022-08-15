#ifndef IOACTORFILE_H
#define IOACTORFILE_H

#include <QObject>
#include "fileparser.h"
#include "RXObjects/rpgactor.h"


class IOActorFile : public FileParser
{
public:
    IOActorFile();
    IOActorFile(QString path, QList<RPGActor*> *actor_list);

    void write_to_file(QString path, QList<RPGActor*> *actor_list);
};

#endif // IOACTORFILE_H
