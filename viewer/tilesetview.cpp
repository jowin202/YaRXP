#include "RXIO2/rpgdb.h"

#include <QScrollBar>

#include "tilesetrectangle.h"
#include "tilesetview.h"


TilesetView::TilesetView(QWidget *parent) : QGraphicsView(parent)
{
    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    this->rectangle = 0;

}

void TilesetView::set_tileset(int id)
{
    QJsonObject current = db->get_tileset_by_id(id);
    QImage tileset_image(db->project_dir + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() + current.value("@tileset_name").toString());
    this->max_height = tileset_image.height()+32;
    QJsonArray autotiles = current.value("@autotile_names").toArray();
    if (tileset_image.isNull())
        return;


    this->scene()->clear();
    this->setBackgroundBrush(db->transparent);

    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap::fromImage(tileset_image));
    background->setPos(0,32);
    this->scene()->addItem(background);
    this->scene()->setSceneRect(0,0,256,max_height);


    for (int i = 0; i < 7; i++)
    {
        QImage autotile_img(db->project_dir + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() + autotiles.at(i).toString());
        QGraphicsPixmapItem *autotile = new QGraphicsPixmapItem(QPixmap::fromImage(autotile_img.copy(0,0,32,32)));
        autotile->setPos((i+1)*32,0);
        this->scene()->addItem(autotile);
    }

    this->rectangle = new TilesetRectangle(1,1);
    this->rectangle->setPos(0,0);
    this->scene()->addItem(this->rectangle);
}

void TilesetView::mouseMoveEvent(QMouseEvent *event)
{
    //return when outside of range and return when autotile
    if (mapToScene(event->pos()).x() <= 0 || mapToScene(event->pos()).x() >= 256
            || mapToScene(event->pos()).y() <= 32 || mapToScene(event->pos()).y() >= max_height) return;

    if (clicked)
    {
        QPointF posf = mapToScene(event->pos());
        QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);

        QRectF br1 = rectangle->boundingRect();

        this->rectangle->setPos(32*QPoint(qMin(pos.x(),last_pos.x()), qMin(pos.y(),last_pos.y())));
        this->rectangle->setSize(1+qAbs(pos.x()-last_pos.x()), 1+qAbs(pos.y()-last_pos.y()));

        QRectF br2 = rectangle->boundingRect();


        this->scene()->update(
                    QRectF(QPointF(qMin(br1.x(),br2.x()), qMin(br1.y(),br2.y())),
                           QSize(qMax(br1.width(),br2.width()), qMax(br1.height(),br2.height()))
                           )
                    );
    }
}

void TilesetView::mousePressEvent(QMouseEvent *event)
{
    if (mapToScene(event->pos()).x() <= 0 || mapToScene(event->pos()).x() >= 256
            || mapToScene(event->pos()).y() <= 0 || mapToScene(event->pos()).y() >= max_height) return;

    if (this->rectangle == 0) return;

    if (mapToScene(event->pos()).y() > 32)
        this->clicked = true; //only allow bigger rectangle for non-autotiles

    QPointF posf = mapToScene(event->pos());
    QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);


    this->last_pos = pos;
    this->rectangle->setPos(32*pos.x(),32*pos.y());
    this->rectangle->setSize(1,1);
    this->rectangle->update();
}

void TilesetView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (this->rectangle == 0) return;

    this->clicked = false;
    QList<int> data;

    if (mapToScene(event->pos()).y() < 32)
    {
        int x = mapToScene(event->pos()).x()/32;

        data.append(1);
        data.append(1);
        data.append(0x30*x);
    }
    else
    {
        data.append(this->rectangle->size().width());
        data.append(this->rectangle->size().height());

        int x = (this->rectangle->pos().x()/32);
        int y = (this->rectangle->pos().y()/32)-1; //autotile

        int w = this->rectangle->size().width();
        int h = this->rectangle->size().height();


        for (int i = 0; i <  w*h; i++)
        {
            data.append(0x180 + 8*y + x + (i % w) + 8*(i/w) );
        }
    }

    emit selection_changed(data);
}

void TilesetView::select_tile(int value)
{
    int x,y;

    if (value >= 0x180)
    {
        value -= 0x180;
        y = 1 + value/8;
        x = value%8;
    }
    else
    {
        value /= 0x30;
        x = value;
        y = 0;
    }

    this->centerOn(128,32*y);
    this->rectangle->setPos(32*x,32*y);
    this->rectangle->setSize(1,1);
    this->rectangle->update();
}
