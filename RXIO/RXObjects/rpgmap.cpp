#include "rpgmap.h"

RPGMap::RPGMap(QObject *parent) : QObject(parent)
{

}

RPGEvent* RPGMap::event_on_pos(QPoint pos)
{
    for (int i = 0; i < events.size(); i++)
    {
        if (events.at(i)->x == pos.x() && events.at(i)->y == pos.y())
            return events.at(i);
    }
    return 0;
}


QImage RPGMap::create_map_image(int zoom, bool dim_other, bool show_current_and_below, int current_layer, RPGTileset *tileset)
{
    bool events = false;
    int img_height = this->height * 32;
    int img_width = this->width * 32;
    QImage result(img_width, img_height, QImage::Format_ARGB32);

    if (current_layer == 3) //events
    {
        current_layer = 2; //there is no layer above 2
        events = true;
    }

    QPainter painter;
    painter.begin(&result);
    painter.fillRect(0,0,img_width, img_height, Qt::black);
    painter.setOpacity(1);


    //over all layers
    for (int layer = 0; layer <= 2; layer++)
    {
        if (!events && !show_current_and_below && dim_other && layer == current_layer)
            painter.setOpacity(1);
        else if (!events && !show_current_and_below && dim_other && layer != current_layer)
            painter.setOpacity(0.5);

        //one layer
        for (int i = 0; i < this->height * this->width; i++)
        {
            int x = i % this->width;
            int y = i/this->width;
            QPoint pos = 32*QPoint(x, y);
            QRect target_rect(pos, pos + QPoint(31,31));


            int index = layer * this->height * this->width + i;
            if (this->data[index] == 0 && layer == 0)
            {
                //if empty and first layer, background is pink
                painter.fillRect(target_rect, QColor(0xaa,0x16,0xa0));
            }
            else if (this->data[index] > 0 && this->data[index] < 0x0180)
            {
                //autotiles
                int autotileset_num = this->data[index] / 48 - 1;
                int autotile_num = this->data[index] % 48;
                QImage tile = tileset->autotiles[autotileset_num].getTile(autotile_num);

                if (!tile.isNull())
                    painter.drawImage(target_rect,tile);
            }
            else{
                QImage tile = tileset->getTile(this->data[index]);
                if (!tile.isNull())
                    painter.drawImage(target_rect, tile);
            }
        }
        if (show_current_and_below && layer == current_layer)
            break;
    }

    if (events)
    {
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(150,150,150));
        painter.setPen(pen);

        for (int y = 0; y < this->height; y++)
            painter.drawLine(0,32*y,this->width*32,32*y);
        for (int x = 0; x < this->width; x++)
            painter.drawLine(32*x,0,32*x,this->height*32);

        painter.setPen(Qt::white);
        for (int i = 0; i < this->events.length(); i++)
        {
            RPGEvent *event = this->events.at(i);

            painter.drawRect(32*event->x+1, 32*event->y+1, 30,30);
            painter.setOpacity(0.5);
            painter.fillRect(32*event->x+1, 32*event->y+1, 30,30, Qt::white);
            painter.setOpacity(1);

            if (event->pages.length() >= 1 && !event->pages.at(0)->graphics.isNull())
            {
                int dir_offset = (event->pages.at(0)->direction / 2) - 1;
                int pattern_offset = event->pages.at(0)->pattern; //0,1,2,3

                //direction: 2 down, 4 left, 6 right, 8 up

                int pic_height = event->pages.at(0)->graphics.size().height()/4; //persons have 48 pixels, pokeballs 32
                painter.drawImage(QRect(32*event->x, 32*event->y, 31,31),event->pages.at(0)->graphics,QRect(32*pattern_offset,pic_height*dir_offset,32,32));
            }

        }

    }


    painter.end();



    if (zoom == ZOOM_50)
        return result.scaled(result.width()/2, result.height()/2,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    else if (zoom == ZOOM_25)
        return result.scaled(result.width()/4, result.height()/4,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    else
        return result;
}
