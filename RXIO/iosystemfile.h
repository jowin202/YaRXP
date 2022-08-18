#ifndef IOSYSTEMFILE_H
#define IOSYSTEMFILE_H

#include <QObject>

#include "fileparser.h"
#include "RXObjects/rpgsystem.h"


class IOSystemFile : public FileParser
{
    Q_OBJECT
public:
    explicit IOSystemFile(QObject *parent = nullptr);

    IOSystemFile(QString path, RPGSystem *settings);

    void write_to_file(QString path, RPGSystem *settings);



signals:

};

#endif // IOSYSTEMFILE_H
