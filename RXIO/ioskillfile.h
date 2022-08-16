#ifndef IOSKILLFILE_H
#define IOSKILLFILE_H

#include <QObject>
#include "fileparser.h"
#include "RXObjects/rpgskill.h"

class IOSKillFile : public FileParser
{
    Q_OBJECT
public:
    IOSKillFile();

    IOSKillFile(QString path, QList<RPGSkill*> *skill_list);


    void write_to_file(QString path, QList<RPGSkill*> *skill_list);
};

#endif // IOSKILLFILE_H
