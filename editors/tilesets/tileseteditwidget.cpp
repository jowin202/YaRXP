#include "tileseteditwidget.h"

#include "RXIO/RXObjects/rpgtileset.h"
#include "RXIO/RXObjects/rpgsystem.h"

TilesetEditWidget::TilesetEditWidget(QWidget *parent) : QGraphicsView(parent)
{
    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void TilesetEditWidget::set_data(QList<int> *passages, QList<int> *priorities, QList<int> *terrain)
{

}


void TilesetEditWidget::set_tileset(QString tileset_image)
{
    this->scene()->clear();

    QImage tileset_img(system->tileset_dir + tileset_image);

    int tiles_x = tileset_img.width()/32; //assuming that is divisible
    int tiles_y = qCeil(tileset_img.height()/32.0);

    tiles_num = tiles_x  * (tiles_y-1) + 48*8;



    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap::fromImage(tileset_img));
    //this->max_height = this->current_tileset->tileset.height()+32;
    background->setPos(0,32);
    this->scene()->addItem(background);


    /*
    for (int i = 0; i < 7; i++)
    {
        QGraphicsPixmapItem *autotile = new QGraphicsPixmapItem(QPixmap::fromImage(current_tileset->autotiles.at(i).thumb));
        autotile->setPos((i+1)*32,0);
        this->scene()->addItem(autotile);
    }
    */

}

void TilesetEditWidget::set_autotile(QString autotile_image, int pos)
{
    QImage autotile_img(system->autotiles_dir + autotile_image);

    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap::fromImage(autotile_img.copy(0,0,32,32)));
    background->setPos(32*pos,0);
    this->scene()->addItem(background);

}
