#include "tileseteditwidget.h"

#include "tile.h"
#include "RXIO/RXObjects/rpgtileset.h"
#include "RXIO/RXObjects/rpgsystem.h"

TilesetEditWidget::TilesetEditWidget(QWidget *parent) : QGraphicsView(parent)
{
    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    this->setMaximumWidth(256);

    this->tiles_num = 0;
    this->opt.mode = Tile::PASSAGES;
}

void TilesetEditWidget::set_data(QList<int> *passages, QList<int> *priorities, QList<int> *terrain)
{
    if (tiles_num > 0)
    {
        this->passages.clear();
        this->priorities.clear();
        this->terrain.clear();

        //8 autotiles
        for (int i = 0; i < 8; i++)
        {
            this->passages.append(passages->at(4 + 0x60 * i));
            this->priorities.append(priorities->at(4 + 0x60 * i));
            this->terrain.append(terrain->at(4 + 0x60 * i));
        }
        //rest of the tiles
        for (int i = 0; i < tiles_num-8; i++)
        {
            this->passages.append(passages->at(4 + 0x60*8 + 2*i));
            this->priorities.append(priorities->at(4 + 0x60*8 + 2*i));
            this->terrain.append(terrain->at(4 + 0x60*8 + 2*i));
        }
    }
}

void TilesetEditWidget::set_mode(int mode)
{
    this->opt.mode = mode;
    this->update();
    this->scene()->update();
}


void TilesetEditWidget::set_tileset(QString tileset_image)
{
    this->scene()->clear();

    this->setBackgroundBrush(system->purple);
    QImage tileset_img(system->tileset_dir + tileset_image);

    int tiles_x = tileset_img.width()/32; //assuming that is divisible
    int tiles_y = qCeil(tileset_img.height()/32.0);
    //assert tiles_x == 8

    tiles_num = tiles_x  * (tiles_y-1) + 48*8;



    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap::fromImage(tileset_img));
    //this->max_height = this->current_tileset->tileset.height()+32;
    background->setPos(0,32);
    this->scene()->addItem(background);

    for (int i = 0; i < 8; i++)
    {
        this->autotiles_items[i] = new QGraphicsPixmapItem;
        this->autotiles_items[i]->setPos(32*i,0);
        this->scene()->addItem(autotiles_items[i]);
    }

    for (int y = 0; y < tiles_y+1; y++) //autotiles
    {
        for (int x = 0; x < tiles_x; x++)
        {
            Tile *tile = new Tile(&opt, y*tiles_x + x, &passages, &priorities, &terrain);
            tile->setPos(32*x,32*y);
            this->scene()->addItem(tile);
        }
    }


}

void TilesetEditWidget::set_autotile(QString autotile_image, int pos)
{
    QImage autotile_img(system->autotiles_dir + autotile_image);

    //QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap::fromImage(autotile_img.copy(0,0,32,32)));
    //background->setPos(32*pos,0);
    //this->scene()->addItem(background);

    this->autotiles_items[pos]->setPixmap(QPixmap::fromImage(autotile_img.copy(0,0,32,32)));
    this->autotiles_items[pos]->update();

}
