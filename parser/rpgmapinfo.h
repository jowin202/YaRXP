#ifndef RPGMAPINFO_H
#define RPGMAPINFO_H

#include <QDebug>
#include <QObject>
#include "rpgmap.h"

class RPGMapInfo : public QObject
{
    Q_OBJECT
public:
    explicit RPGMapInfo(QObject *parent = nullptr);
    void set_id(int id);

    void debug();

    int id;
    bool expanded;
    int parent_id;
    int scroll_x;
    int scroll_y;
    int order;
    QString name;
    RPGMap *map = 0;

signals:

private:


};

#endif // RPGMAPINFO_H
