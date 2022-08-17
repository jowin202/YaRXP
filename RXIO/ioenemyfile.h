#ifndef IOENEMYFILE_H
#define IOENEMYFILE_H

#include <QObject>
#include "RXObjects/rpgenemy.h"
#include "fileparser.h"

class IOEnemyFile : public FileParser
{
    Q_OBJECT
public:
    IOEnemyFile();
    IOEnemyFile(QString path, QList<RPGEnemy*> *enemy_list);

    void write_to_file(QString path, QList<RPGEnemy*> *enemy_list);

};

#endif // IOENEMYFILE_H
