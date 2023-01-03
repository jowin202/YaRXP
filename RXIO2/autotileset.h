#ifndef AUTOTILESET_H
#define AUTOTILESET_H

#include <QObject>
#include <QDebug>

#include <QImage>
#include <QPainter>

class Autotileset
{
public:
    Autotileset();
    Autotileset(QImage tileset);

    QImage create_tile(int n1, int n2, int n3, int n4);
    QImage getTile(int tile);

    QImage get_full_tileset() { return this->tileset_full; }

    int get_id_by_neighbourhood(int n);


    QImage tileset;
    QImage tileset_full;
    QImage thumb;
    QList<QImage> subimage_list;

private:
};

#endif // AUTOTILESET_H
