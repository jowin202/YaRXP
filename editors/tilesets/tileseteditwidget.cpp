#include "tileseteditwidget.h"

#include "tile.h"
#include "RXIO2/rpgdb.h"
#include "RXIO2/fileopener.h"
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
    this->opt.parent = this; //change callbacks
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
    QImage tileset_img = FileOpener(ec->get_db()->tileset_dir,tileset_image).get_image();

    int tiles_x = 8; //tileset_img.width()/32; //assuming that is divisible
    int tiles_y = qFloor(tileset_img.height()/32.0);
    //assert tiles_x == 8

    tiles_num = tiles_x  * (tiles_y-1) + 48*8;

    this->scene()->setSceneRect(0,0,256,tileset_img.height()+32);
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
            QImage autotile_img = FileOpener(ec->get_db()->autotiles_dir,autotile_names.at(i-1).toString()).get_image();
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

void TilesetEditWidget::change_passage(int pos, int val)
{
    QJsonObject passages_obj = ec->obj_get_jsonvalue(RPGDB::TILESETS, "@passages").toObject();
    QJsonArray passages = passages_obj.value("values").toArray();

    if (pos < 8)
    {
        for (int i = 0; i < 48; i++)
        {
            passages.removeAt(48*pos + i);
            passages.insert(48*pos + i, val);
        }
    }
    else
    {
        pos -= 8;
        pos += 8*48;
        passages.removeAt(pos);
        passages.insert(pos, val);
    }

    passages_obj.insert("values", passages);
    ec->obj_set_jsonvalue(RPGDB::TILESETS, "@passages", passages_obj);
}

void TilesetEditWidget::change_priority(int pos, int val)
{
    QJsonObject priorities_obj = ec->obj_get_jsonvalue(RPGDB::TILESETS, "@priorities").toObject();
    QJsonArray priorities = priorities_obj.value("values").toArray();

    if (pos < 8)
    {
        for (int i = 0; i < 48; i++)
        {
            priorities.removeAt(48*pos + i);
            priorities.insert(48*pos + i, val);
        }
    }
    else
    {
        pos -= 8;
        pos += 8*48;
        priorities.removeAt(pos);
        priorities.insert(pos, val);
    }
    priorities_obj.insert("values", priorities);
    ec->obj_set_jsonvalue(RPGDB::TILESETS, "@priorities", priorities_obj);
}

void TilesetEditWidget::change_terrain(int pos, int val)
{
    QJsonObject terrain_tags_obj = ec->obj_get_jsonvalue(RPGDB::TILESETS, "@terrain_tags").toObject();
    QJsonArray terrain_tags = terrain_tags_obj.value("values").toArray();

    if (pos < 8)
    {
        for (int i = 0; i < 48; i++)
        {
            terrain_tags.removeAt(48*pos + i);
            terrain_tags.insert(48*pos + i, val);
        }
    }
    else
    {
        pos -= 8;
        pos += 8*48;
        terrain_tags.removeAt(pos);
        terrain_tags.insert(pos, val);
    }
    terrain_tags_obj.insert("values", terrain_tags);
    ec->obj_set_jsonvalue(RPGDB::TILESETS, "@terrain_tags", terrain_tags_obj);
}

