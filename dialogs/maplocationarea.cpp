#include "maplocationarea.h"

MapLocationArea::MapLocationArea(QWidget *parent) : QGraphicsView(parent)
{

    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

}

void MapLocationArea::mousePressEvent(QMouseEvent *e)
{
    if (rectangle != 0)
        this->rectangle->setPos(e->pos().x()/32*32, e->pos().y()/32*32);
    emit pressed(e->pos().x()/32, e->pos().y()/32);
}
