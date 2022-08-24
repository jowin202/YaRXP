#ifndef RPGMAPINFO_H
#define RPGMAPINFO_H

#include <QDebug>
#include <QObject>
#include <QFile>
#include "rpgmap.h"
#include "rpgstring.h"

class RPGSystem;

class RPGMapInfo : public QObject
{
    Q_OBJECT
public:
    explicit RPGMapInfo(QObject *parent = nullptr);

    void set_id(int id);
    void load_map(RPGSystem *system);
    void save_map(RPGSystem *system);

    void debug();

    int id;
    bool expanded;
    int parent_id;
    int scroll_x;
    int scroll_y;
    int order;
    RPGString name;
    RPGMap *map = 0;

signals:

private:


};

#endif // RPGMAPINFO_H
