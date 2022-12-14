#include "tile.h"

Tile::Tile(tile_options *opt, int tile_num, int passage, int priority, int terrain)
{
    this->opt = opt;
    this->tile_num = tile_num;

    this->passage = passage;
    this->priority = priority;
    this->terrain = terrain;
}

QRectF Tile::boundingRect() const
{
    return QRectF(0,0,32,32);
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QPainterPath path;


    painter->setOpacity(0.8);
    painter->drawLine(0,0,32,0);
    painter->drawLine(0,0,0,32);
    if (opt->mode == PASSAGES)
    {

        if ((passage & 0xF) == 0xF)
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
        painter->setOpacity(1);
        painter->setFont(font);
        painter->drawText(QRect(0,0,32,32), Qt::AlignHCenter | Qt::AlignVCenter, QString::number(priority));
    }
    else if (opt->mode == BUSH)
    {
        if ((passage & 0x40) != 0)
        {
            QPen pen;
            pen.setColor(Qt::white);
            pen.setWidth(4);
            painter->setPen(pen);
            painter->drawRect(8,8,16,16);
            pen.setColor(Qt::black);
            painter->setPen(pen);
            painter->drawRect(5,5,22,22);
        }
    }
    else if (opt->mode == COUNTER)
    {
        if ((passage & 0x80) != 0)
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
        painter->setOpacity(1);
        painter->drawText(QRect(0,0,32,32), Qt::AlignHCenter | Qt::AlignVCenter, QString::number(terrain));
    }

}


void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
/*
    if (event->button() == Qt::RightButton) return;

    if (opt->mode == PASSAGES)
    {
        int val = passage);
        if (val == 0xF)
            passages->replace(tile_num, val & 0xFFFFF0); //32 bit integer assumed
        else
            passages->replace(tile_num, val | 0xF);
    }
    else if (opt->mode == PASSAGES4)
    {

    }
    else if (opt->mode == PRIORITY)
    {
        int val = priorities->at(tile_num);
        if (val == 5) val = 0;
        else val++;
        priorities->replace(tile_num, val);
    }
    else if (opt->mode == BUSH)
    {
        int val = passages->at(tile_num);
        passages->replace(tile_num, val ^ 0x40);
    }
    else if (opt->mode == COUNTER)
    {
        int val = passages->at(tile_num);
        passages->replace(tile_num, val ^ 0x80);
    }
    else if (opt->mode == TERRAIN)
    {
        int val = terrain->at(tile_num);
        if (val == 31) val = 0;
        else val++;
        terrain->replace(tile_num, val);
    }
*/
    this->update();
}
