#ifndef IOANIMATIONFILE_H
#define IOANIMATIONFILE_H

#include <QObject>

#include "fileparser.h"
#include "RXObjects/rpganimation.h"


class IOAnimationFile : public FileParser
{
    Q_OBJECT
public:
    IOAnimationFile();
    IOAnimationFile(QString path, QList<RPGAnimation*> *animation_list);

    void write_to_file(QString path, QList<RPGAnimation*> *animation_list);
};

#endif // IOANIMATIONFILE_H
