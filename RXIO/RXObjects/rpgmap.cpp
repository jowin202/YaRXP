#include "rpgmap.h"
#include "rpgsystem.h"

RPGMap::RPGMap(QObject *parent) : QObject(parent)
{

}

RPGMap::~RPGMap()
{
    qDebug() << "Map deleted";
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

RPGEvent *RPGMap::event_by_id(int id)
{
    for (int i = 0; i < events.size(); i++)
    {
        if (events.at(i)->id == id)
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
                QImage tile = tileset->getAutoTile(autotileset_num, autotile_num);


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

QList<int> RPGMap::get_elements_in_rectangle(QRect rect, int fromlayer, int tolayer)
{
    QList<int> result;
    result.append(rect.width());
    result.append(rect.height());


    for (int l = fromlayer; l <= tolayer; l++)
    {
        for (int y = rect.topLeft().y(); y <= rect.bottomRight().y(); y++)
        {
            for (int x = rect.topLeft().x(); x <= rect.bottomRight().x(); x++)
            {
                int pos = array_position(QPoint(x,y),l);
                if (pos < this->data.length())
                    result.append(this->data[pos]);
            }
        }
    }

    return result;
}

void RPGMap::delete_elements_in_rectangle(QRect rect, int fromlayer, int tolayer)
{
    for (int l = fromlayer; l <= tolayer; l++)
    {
        for (int y = rect.topLeft().y(); y <= rect.bottomRight().y(); y++)
        {
            for (int x = rect.topLeft().x(); x <= rect.bottomRight().x(); x++)
            {
                int pos = array_position(QPoint(x,y),l);
                if (pos < this->data.length())
                    this->data[pos] = 0;
            }
        }
    }
}

void RPGMap::put_elements_from_list(QPoint pos, QPoint rel_pos, QList<int> list, int fromlayer, int tolayer)
{
    if (list.length() <= 2) return;

    int width = list.first(); list.pop_front();
    int height = list.first(); list.pop_front();
    int xr = rel_pos.x();
    int yr = rel_pos.y();

    for (int i = 0; i < list.length(); i++)
    {
        for (int l = fromlayer; l <= tolayer; l++)
        {
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    QPoint pos_on_map = pos + QPoint(x,y);
                    if (pos_on_map.x() >= this->width || pos_on_map.x() < 0) continue;
                    if (pos_on_map.y() >= this->height || pos_on_map.y() < 0) continue;
                    int pos_in_array = array_position(pos_on_map,l);

                    //thx to MW
                    int xe = (x+xr)%width;
                    int ye = (y+yr)%height;
                    if (xe < 0) xe += width;
                    if (ye < 0) ye += height;

                    this->data[pos_in_array] = list[ xe + width*ye + width*height*(l-fromlayer)];
                }
            }
        }
    }

}

void RPGMap::move_map_part(QRect rect, QPoint pos)
{
    if (rect.topLeft() == pos) return;

    QPoint move_vector = pos - rect.topLeft();
    QList<int > temp;

    for (int l = 0; l <= 2; l++)
    {
        for (int y = rect.topLeft().y(); y <= rect.bottomRight().y(); y++)
        {
            for (int x = rect.topLeft().x(); x <= rect.bottomRight().x(); x++)
            {
                QPoint source_pos(x,y);
                QPoint target_pos = source_pos + move_vector;

                if (target_pos.x() < 0 || target_pos.x() >= width) continue;
                if (target_pos.y() < 0 || target_pos.y() >= height) continue;
                int source_index = array_position(source_pos,l);
                temp.append(this->data[source_index]);
                this->data[source_index] = 0;
            }
        }
    }
    for (int l = 0; l <= 2; l++)
    {
        for (int y = rect.topLeft().y(); y <= rect.bottomRight().y(); y++)
        {
            for (int x = rect.topLeft().x(); x <= rect.bottomRight().x(); x++)
            {
                QPoint source_pos(x,y);
                QPoint target_pos = source_pos + move_vector;

                if (target_pos.x() < 0 || target_pos.x() >= width) continue;
                if (target_pos.y() < 0 || target_pos.y() >= height) continue;


                int target_index = array_position(target_pos,l);

                this->data[target_index] = temp.first(); temp.pop_front();
            }
        }
    }

    //move events
    for (int i = 0; i < this->events.length(); i++)
    {
        if (rect.contains(QPoint(events.at(i)->x, events.at(i)->y)))
        {
            events.at(i)->x += move_vector.x();
            events.at(i)->y += move_vector.y();
        }
    }
}

void RPGMap::shift_map_x(int tiles)
{
    tiles = tiles % width;

    for (int i = 0; i < events.size(); i++)
    {
        events.at(i)->x += tiles;
        if (events.at(i)->x < 0) this->events.at(i)->x += width;
        else if (events.at(i)->x >= width) this->events.at(i)->x -= width;
    }

    QList<int> tmp = QList<int>(this->data); //copy
    int newpos;

    for (int layer = 0; layer <= 2; layer++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                newpos = (x+tiles) % width;
                if (newpos < 0) newpos += width;

                this->data[array_position(QPoint(newpos,y),layer)] = tmp[array_position(QPoint(x,y),layer)];
            }
        }
    }
}

void RPGMap::shift_map_y(int tiles)
{
    tiles = tiles % height;

    for (int i = 0; i < events.size(); i++)
    {
        events.at(i)->y += tiles;
        if (events.at(i)->y < 0) this->events.at(i)->y += height;
        else if (events.at(i)->y >= height) this->events.at(i)->y -= height;
    }

    QList<int> tmp = QList<int>(this->data); //copy
    int newpos;

    for (int layer = 0; layer <= 2; layer++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                newpos = (y+tiles) % height;
                if (newpos < 0) newpos += height;

                this->data[array_position(QPoint(x,newpos),layer)] = tmp[array_position(QPoint(x,y),layer)];
            }
        }
    }

}

void RPGMap::extend_or_crop_x(int tiles)
{
    if (tiles < 0) // remove some events
    {
        for (int i = events.size()-1; i >= 0; i--)
        {
            if (events.at(i)->x >= width+tiles)
            {
                delete events.at(i);
                events.removeAt(i);
            }
        }

        for (int layer = 2; layer >= 0; layer--)
            for (int y = height-1; y >= 0; y--)
                for (int x = 0; x < -tiles; x++)
                    this->data.removeAt(array_position(QPoint(width-1-x,y),layer));
        this->width += tiles;
    }
    else //extend
    {
        this->width += tiles;
        for (int layer = 0; layer <= 2; layer++)
            for (int y = 0; y < height; y++)
                for (int x = 0; x < tiles; x++)
                    this->data.insert(array_position(QPoint(width-1,y),layer), 0);
    }

}

void RPGMap::extend_or_crop_y(int tiles)
{
    if (tiles < 0) // remove some events
    {
        for (int i = events.size()-1; i >= 0; i--)
        {
            if (events.at(i)->y >= height+tiles)
            {
                delete events.at(i);
                events.removeAt(i);
            }
        }
        for (int layer = 2; layer >= 0; layer--)
            for (int y = 0; y < -tiles; y++)
                for (int x = width-1; x >= 0; x--)
                    this->data.removeAt(array_position(QPoint(x,height-1-y),layer));
        this->height += tiles;
    }
    else //extend
    {
        this->height += tiles;
        for (int layer = 0; layer <= 2; layer++)
            for (int y = 0; y < tiles; y++)
                for (int x = 0; x < width; x++)
                    this->data.insert(array_position(QPoint(x,height-1),layer), 0);
    }
}

void RPGMap::load_event_graphics(RPGSystem *system)
{
    for (int i = 0; i < this->events.length(); i++)
    {
        for (int j = 0; j < this->events.at(i)->pages.length(); j++)
        {
            if (this->events.at(i)->pages.at(j)->graphics.isNull())
            {
                QString character_name = this->events.at(i)->pages.at(j)->character_name;
                this->events.at(i)->pages.at(j)->graphics = QImage(system->characters_dir + character_name);
            }
        }
    }
}
