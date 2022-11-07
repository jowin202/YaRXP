#ifndef MAPRECTANGLE_H
#define MAPRECTANGLE_H

#include <QPainter>
#include <QGraphicsItem>

class MapRectangle : public QGraphicsItem
{
public:
    MapRectangle(int w, int h);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QRectF boundingRect() const override;
    void setSize(int w, int h) {this->prepareGeometryChange(); this->w = w; this->h = h; }

    int width() {return this->w;}
    int height() {return this->h;}

private:
    int w, h;
};

#endif // MAPRECTANGLE_H
