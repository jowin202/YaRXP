#include "mapview.h"
#include "maprectangle.h"
#include "maptile.h"
#include "RXIO/RXObjects/rpgmap.h"
#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgtileset.h"
#include "RXIO/RXObjects/parserexception.h"

MapView::MapView(QWidget *parent) : QGraphicsView(parent)
{
    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    this->map = 0;
    this->opt.layer = 0;
    this->opt.current_and_below = false;
    //this->opt.dim = true;
    this->opt.mode = PEN;

}

void MapView::set_map(int id)
{
    try {
        this->system->map_info_list.at(id)->load_map(this->system);
    } catch (ParserException *ex) {
        qDebug() << ex->error_data << ex->message;
    }
    this->map = this->system->map_info_list.at(id)->map;


    if (this->system->tileset_hash.contains(map->tileset_id))
        this->tileset = this->system->tileset_hash.value(map->tileset_id);


    this->changes_made = false;
    this->redraw();
}

void MapView::set_layer(int layer)
{
    this->opt.layer = layer;
    this->scene()->update();
    //this->redraw();
    //TODO choose pen when leaving event mode
}

void MapView::set_current_and_below(bool val)
{
    this->opt.current_and_below = val;
    this->scene()->update();
}

void MapView::set_brush(QList<int> data)
{
    this->brush = data;
    this->rectangle_offset = QPoint(0,0);
    this->rectangle->setSize(data.at(0), data.at(1));
    this->rectangle->update();
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    if (map == 0 || this->rectangle == 0) return;
    if (mapToScene(event->pos()).x() <= 0 || mapToScene(event->pos()).x() >= 32*map->width) return;
    if (mapToScene(event->pos()).y() <= 0) return;

    QPointF posf = mapToScene(event->pos());
    QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);
    if (pos == last_pos) return;
    this->last_pos = pos;

    if (opt.mode == PEN)
    {
        //workaround that rectangle does not go over topleft
        if ((pos+rectangle_offset).x() < 0)  pos.setX(-rectangle_offset.x());
        if ((pos+rectangle_offset).y() < 0)  pos.setY(-rectangle_offset.y());
        if ((pos+rectangle_offset).x()+rectangle->width() >= map->width)  pos.setX(map->width-rectangle_offset.x()-rectangle->width());
        if ((pos+rectangle_offset).y()+rectangle->height() >= map->height)  pos.setY(map->height-rectangle_offset.y()-rectangle->height());


        if (this->left_button_clicked)
        {
            QPoint rel_pos = pos - this->draw_from_pos;
            this->map->put_elements_from_list(pos+rectangle_offset, rel_pos, this->brush, this->opt.layer, this->opt.layer);
        }
        else if (this->right_button_clicked)
        {
            QPoint topleft = QPoint(qMin(pos.x(),this->rightclick_from_pos.x()),qMin(pos.y(),this->rightclick_from_pos.y()));
            QPoint bottomright = QPoint(qMax(pos.x(),this->rightclick_from_pos.x()),qMax(pos.y(),this->rightclick_from_pos.y()));
            this->set_brush(this->map->get_elements_in_rectangle(QRect(topleft,bottomright),opt.layer,opt.layer));
            pos = topleft; //overwrite pos to topleft. rectangle should not move while rightclick
        }

        rectangle->setPos(32*(pos+rectangle_offset));
        rectangle->update();
    }
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (map == 0 || this->rectangle == 0) return;

    QPointF posf = mapToScene(event->pos());
    QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);


    if (opt.mode == PEN)
    {
        //workaround that rectangle does not go over topleft
        if ((pos+rectangle_offset).x() < 0) pos.setX(-rectangle_offset.x());
        if ((pos+rectangle_offset).y() < 0) pos.setY(-rectangle_offset.y());
        if ((pos+rectangle_offset).x()+rectangle->width() >= map->width)  pos.setX(map->width-rectangle_offset.x()-rectangle->width());
        if ((pos+rectangle_offset).y()+rectangle->height() >= map->height)  pos.setY(map->height-rectangle_offset.y()-rectangle->height());

        if (event->button() == Qt::LeftButton)
        {
            this->left_button_clicked = true;
            this->draw_from_pos = pos;
            this->map->put_elements_from_list(pos+rectangle_offset, QPoint(0,0), this->brush, this->opt.layer, this->opt.layer);
            this->rectangle->update();
        }
        else if (event->button() == Qt::RightButton)
        {
            this->rectangle->setPos(32*pos);
            this->right_button_clicked = true;
            this->rightclick_from_pos = pos;
            this->set_brush(this->map->get_elements_in_rectangle(QRect(pos.x(),pos.y(),1,1),this->opt.layer,this->opt.layer));
            this->rectangle->update();
        }
    }
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF posf = mapToScene(event->pos());
    QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);

    if (event->button() == Qt::LeftButton)
        this->left_button_clicked = false;

    if (event->button() == Qt::RightButton)
    {
        this->right_button_clicked = false;
        this->rectangle_offset = QPoint(this->rectangle->pos().x()/32,this->rectangle->pos().y()/32)-pos;
    }
}

void MapView::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void MapView::wheelEvent(QWheelEvent *event)
{
    if ((event->modifiers() & Qt::Modifier::CTRL) != 0)
    {
        if (event->delta() > 0)
            emit zoom_in();
        else emit zoom_out();

        return;
    }
    event->accept();
    QGraphicsView::wheelEvent(event);
}

void MapView::redraw()
{
    this->scene()->clear();

    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            MapTile *tile = new MapTile(map, this->tileset, QPoint(x,y), &opt);
            tile->setPos(QPoint(32*x,32*y));
            this->scene()->addItem(tile);
        }
    }

    this->rectangle_offset = QPoint(0,0);
    this->rectangle = new MapRectangle(1,1);
    this->scene()->addItem(this->rectangle);

}
