#ifndef IOSYSTEMFILE_H
#define IOSYSTEMFILE_H

#include <QObject>

#include "fileparser.h"
#include "RXObjects/rpgsettings.h"


class IOSystemFile : public FileParser
{
    Q_OBJECT
public:
    explicit IOSystemFile(QObject *parent = nullptr);

    IOSystemFile(QString path, RPGSettings *settings);

    void write_to_file(QString path, RPGSettings *settings);



signals:

};

#endif // IOSYSTEMFILE_H
