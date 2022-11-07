#ifndef TILESETRECTANGLE_H
#define TILESETRECTANGLE_H

#include <QObject>
#include <QWidget>
#include <QGraphicsItem>
#include <QPainter>

class TilesetRectangle : public QGraphicsItem
{
public:
    TilesetRectangle(int w, int h);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    void setSize(int w, int h) {this->prepareGeometryChange(); this->w = w; this->h = h; }
    QSize size() { return QSize(w,h); }


private:
    int w, h;
};

#endif // TILESETRECTANGLE_H
