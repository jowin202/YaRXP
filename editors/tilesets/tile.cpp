#include "tile.h"
#include "tileseteditwidget.h"

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
        if ((passage & 0x01) != 1) //down
        {
            QPolygon polygon;
            polygon.append(QPoint(10,20));
            polygon.append(QPoint(20,20));
            polygon.append(QPoint(15,30));
            painter->setBrush(Qt::white);
            painter->drawPolygon(polygon);
        }
        if ((passage & 0x02) != 2) //left
        {
            QPolygon polygon;
            polygon.append(QPoint(10,10));
            polygon.append(QPoint(10,20));
            polygon.append(QPoint(1,15));
            painter->setBrush(Qt::white);
            painter->drawPolygon(polygon);
        }
        if ((passage & 0x04) != 4) //right
        {
            QPolygon polygon;
            polygon.append(QPoint(20,10));
            polygon.append(QPoint(20,20));
            polygon.append(QPoint(30,15));
            painter->setBrush(Qt::white);
            painter->drawPolygon(polygon);
        }
        if ((passage & 0x08) != 8) //up
        {
            QPolygon polygon;
            polygon.append(QPoint(10,10));
            polygon.append(QPoint(20,10));
            polygon.append(QPoint(15,1));
            painter->setBrush(Qt::white);
            painter->drawPolygon(polygon);
        }
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
    if (event->button() == Qt::RightButton) return;

    if (opt->mode == PASSAGES)
    {
        int val = passage & 0xF;
        if (val == 0xF)
            passage = val & 0xFFFFF0;
        else
            passage = val | 0xF;
        opt->parent->change_passage(tile_num, passage);
    }
    else if (opt->mode == PASSAGES4)
    {
        if (event->pos().y() < 10 && event->pos().x() > 5 && event->pos().x() < 27)
            passage ^= 0x08;
        if (event->pos().y() > 20 && event->pos().x() > 5 && event->pos().x() < 27)
            passage ^= 0x01;
        if (event->pos().x() < 10 && event->pos().y() > 5 && event->pos().y() < 27)
            passage ^= 0x02;
        if (event->pos().x() > 20 && event->pos().y() > 5 && event->pos().y() < 27)
            passage ^= 0x04;
        opt->parent->change_passage(tile_num, passage);
    }
    else if (opt->mode == PRIORITY)
    {
        if (priority == 5)
            priority = 0;
        else
            priority++;
        opt->parent->change_priority(tile_num, priority);
    }
    else if (opt->mode == BUSH)
    {
        passage = passage ^ 0x40;
        opt->parent->change_passage(tile_num, passage);
    }
    else if (opt->mode == COUNTER)
    {
        passage = passage ^ 0x80;
        opt->parent->change_passage(tile_num, passage);
    }
    else if (opt->mode == TERRAIN)
    {
        if (terrain == 31)
            terrain = 0;
        else
            terrain++;
        opt->parent->change_terrain(tile_num, terrain);
    }
    this->update();
}
