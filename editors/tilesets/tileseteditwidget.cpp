#include "tileseteditwidget.h"

#include "tile.h"
#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

TilesetEditWidget::TilesetEditWidget(QWidget *parent) : QGraphicsView(parent)
{
    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    this->horizontalScrollBar()->setVisible(false);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


    this->viewport()->setMaximumWidth(256);
    this->viewport()->setMinimumWidth(256);
    this->setMinimumWidth(290);
    this->setMaximumWidth(290);

    this->tiles_num = 0;
    this->opt.mode = Tile::PASSAGES;
}


void TilesetEditWidget::set_mode(int mode)
{
    this->opt.mode = mode;
    this->update();
    this->scene()->update();
}

void TilesetEditWidget::update_tileset()
{
    QString tileset_image = ec->obj_get_jsonvalue(RPGDB::TILESETS, "@tileset_name").toString();
    QJsonArray autotile_names = ec->obj_get_jsonvalue(RPGDB::TILESETS, "@autotile_names").toArray();
    QJsonArray passages = ec->obj_get_jsonvalue(RPGDB::TILESETS, "@passages").toObject().value("values").toArray();
    QJsonArray priorities = ec->obj_get_jsonvalue(RPGDB::TILESETS, "@priorities").toObject().value("values").toArray();
    QJsonArray terrain_tags = ec->obj_get_jsonvalue(RPGDB::TILESETS, "@terrain_tags").toObject().value("values").toArray();


    this->scene()->clear();
    this->setBackgroundBrush(ec->get_db()->transparent);
    QImage tileset_img(ec->get_db()->project_dir + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() + tileset_image);

    int tiles_x = 8; //tileset_img.width()/32; //assuming that is divisible
    int tiles_y = qCeil(tileset_img.height()/32.0);
    //assert tiles_x == 8

    tiles_num = tiles_x  * (tiles_y-1) + 48*8;


    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap::fromImage(tileset_img));
    //this->max_height = this->current_tileset->tileset.height()+32;
    background->setPos(0,32);
    this->scene()->addItem(background);
    //this->viewport()->setMaximumHeight(tileset_img.height()+32);

    for (int i = 0; i < 8; i++)
    {
        this->autotiles_items[i] = new QGraphicsPixmapItem;
        this->autotiles_items[i]->setPos(32*i,0);
        if (i > 0)
        {
            QImage autotile_img(ec->get_db()->project_dir + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() + autotile_names.at(i-1).toString());
            if (!autotile_img.isNull())
                this->autotiles_items[i]->setPixmap(QPixmap::fromImage(autotile_img.copy(0,0,32,32)));
        }
        this->scene()->addItem(autotiles_items[i]);
    }

    for (int y = 0; y < tiles_y+1; y++) //autotiles
    {
        for (int x = 0; x < tiles_x; x++)
        {
            int tile_num = y*tiles_x + x;
            int adjusted_tile_num = (y==0 ? 48*x : tile_num + 48*8 - 8); //adjusted for autotiles
            Tile *tile = new Tile(&opt, tile_num, passages.at(adjusted_tile_num).toInt(), priorities.at(adjusted_tile_num).toInt(), terrain_tags.at(adjusted_tile_num).toInt());
            tile->setPos(32*x,32*y);
            this->scene()->addItem(tile);
        }
    }
}

