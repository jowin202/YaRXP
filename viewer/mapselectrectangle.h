#ifndef MAPSELECTRECTANGLE_H
#define MAPSELECTRECTANGLE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QDebug>
#include "RXIO/RXObjects/rpgmap.h"

class RPGTileset;
struct tile_options;

class MapSelectRectangle : public QGraphicsItem
{
public:
    MapSelectRectangle(int w, int h);
    ~MapSelectRectangle();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QRectF boundingRect() const override;
    void setSize(int w, int h) {this->prepareGeometryChange(); this->w = w; this->h = h; }

    bool pos_is_in_rectangle(QPoint pos);


    int width() {return this->w;}
    int height() {return this->h;}

    QSize size() { return QSize(w,h); }

    void set_brush(QList<int> brush, RPGTileset *tileset, tile_options *options);
    QList<int> get_brush() { return this->brush; }

    void save_to_map(RPGMap *map)
    {
        QPoint topleft = QPoint(this->x()/32, this->y()/32);
        map->put_elements_from_list(topleft, QPoint(0,0),this->get_brush(),0,2);
    }


    QList<int> brush;
    RPGTileset *tileset = 0;
    tile_options *opt = 0;

private:
    int w,h;

};

#endif // MAPSELECTRECTANGLE_H
