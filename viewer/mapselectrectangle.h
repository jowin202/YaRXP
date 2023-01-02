#ifndef MAPSELECTRECTANGLE_H
#define MAPSELECTRECTANGLE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include <QPen>
#include <QPainter>
#include <QDebug>


class RPGMapController;

struct tile_options;

class MapSelectRectangle : public QGraphicsItem
{
public:
    MapSelectRectangle(RPGMapController *mc, int w, int h);
    ~MapSelectRectangle();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QRectF boundingRect() const override;
    void setSize(int w, int h) {this->prepareGeometryChange(); this->w = w; this->h = h; }

    bool pos_is_in_rectangle(QPoint pos);


    int width() {return this->w;}
    int height() {return this->h;}

    QSize size() { return QSize(w,h); }

    void set_brush(QList<int> brush, tile_options *options);
    QList<int> get_brush() { return this->brush; }

    void save_to_map();


    QList<int> brush;
    tile_options *opt = 0;

private:
    int w,h;
    RPGMapController *mc;

};

#endif // MAPSELECTRECTANGLE_H
