#include "maplocationarea.h"

MapLocationArea::MapLocationArea(QWidget *parent) : QGraphicsView(parent)
{

    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

}

void MapLocationArea::mousePressEvent(QMouseEvent *e)
{
    QPointF pos = mapToScene(QPoint(e->pos().x(), e->pos().y()));

    if (rectangle != 0)
        this->rectangle->setPos(((int)pos.x()/32)*32, ((int)pos.y()/32)*32);
    emit pressed(pos.x()/32, pos.y()/32);
}
