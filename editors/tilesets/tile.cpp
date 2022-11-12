#include "tile.h"

Tile::Tile(tile_options *opt, int tile_n, QList<int> *passages, QList<int> *priorities, QList<int> *terrain)
{
    this->opt = opt;
    this->passages = passages;
    this->priorities = priorities;
    this->terrain = terrain;
    this->tile_num = tile_n;
}

QRectF Tile::boundingRect() const
{
    return QRectF(0,0,32,32);
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);



    painter->setOpacity(0.8);
    painter->drawLine(0,0,32,0);
    painter->drawLine(0,0,0,32);
    if (opt->mode == PASSAGES)
    {

        if ((passages->at(tile_num) & 0xF) != 0)
        {
            //X
            QPen pen;
            pen.setWidth(4);
            pen.setColor(Qt::black);
            painter->setPen(pen);
            painter->drawLine(10,10,21,21);
            painter->drawLine(10,21,21,10);
            pen.setWidth(2);
            pen.setColor(Qt::white);
            painter->setPen(pen);
            painter->drawLine(10,10,21,21);
            painter->drawLine(10,21,21,10);
        }
        else
        {
            //O
            QPen pen;
            pen.setWidth(4);
            pen.setColor(Qt::black);
            painter->setPen(pen);
            painter->drawEllipse(10,10,10,10);
            pen.setWidth(2);
            pen.setColor(Qt::white);
            painter->setPen(pen);
            painter->drawEllipse(10,10,10,10);
        }
    }
    else if (opt->mode == PASSAGES4)
    {
        QImage arrow_down(":/icons/bxs-down-arrow-alt.svg");
    }
    else if (opt->mode == PRIORITY)
    {
        QPen pen;
        pen.setColor(Qt::white);
        painter->setPen(pen);
        QFont font = painter->font();
        font.setBold(true);
        font.setPixelSize(20);
        painter->setFont(font);
        painter->drawText(QRect(0,0,32,32), Qt::AlignHCenter | Qt::AlignVCenter, QString::number(priorities->at(tile_num)));
    }
    else if (opt->mode == BUSH)
    {
        if ((passages->at(tile_num) & 0x40) != 0)
        {
            QPen pen;
            pen.setColor(Qt::white);
            painter->setPen(pen);
            painter->drawText(10,10, "~");
        }
    }
    else if (opt->mode == COUNTER)
    {
        if ((passages->at(tile_num) & 0x80) != 0)
        {
            QPolygon polygon;
            polygon.append(QPoint(15,4));
            polygon.append(QPoint(26,15));
            polygon.append(QPoint(15,26));
            polygon.append(QPoint(4,15));
            painter->setBrush(Qt::white);
            painter->drawPolygon(polygon);
        }
    }
    else if (opt->mode == TERRAIN)
    {
        QPen pen;
        pen.setColor(Qt::white);
        painter->setPen(pen);
        QFont font = painter->font();
        font.setBold(true);
        font.setPixelSize(20);
        painter->setFont(font);
        painter->drawText(QRect(0,0,32,32), Qt::AlignHCenter | Qt::AlignVCenter, QString::number(terrain->at(tile_num)));
    }

}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}
