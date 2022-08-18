#ifndef IOSTATEFILE_H
#define IOSTATEFILE_H

#include <QObject>
#include "fileparser.h"
#include "RXObjects/rpgstate.h"


class IOStateFile : public FileParser
{
    Q_OBJECT
public:
    IOStateFile();
    IOStateFile(QString path, QList<RPGState*> *state_list);

    void write_to_file(QString path, QList<RPGState *> *state_list);

};

#endif // IOSTATEFILE_H
