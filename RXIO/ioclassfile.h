#ifndef IOCLASSFILE_H
#define IOCLASSFILE_H

#include <QObject>


#include "fileparser.h"
#include "RXObjects/rpgclass.h"


class IOClassFile : public FileParser
{
    Q_OBJECT
public:
    IOClassFile();
    IOClassFile(QString path, QList<RPGClass*> *class_list);

    void write_to_file(QString path, QList<RPGClass*> *class_list);

};

#endif // IOCLASSFILE_H
