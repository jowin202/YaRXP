#ifndef AUTOTILESET_H
#define AUTOTILESET_H

#include <QObject>
#include <QDebug>

#include <QImage>
#include <QPainter>

class Autotileset
{
public:
    Autotileset(QImage tileset);

    QImage create_tile(int n1, int n2, int n3, int n4);



    QImage tileset;
    QImage tileset_full;
    QImage thumb;
    QList<QImage> subimage_list;

private:
};

#endif // AUTOTILESET_H
