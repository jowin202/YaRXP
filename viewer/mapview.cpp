#include "mapview.h"
#include "maprectangle.h"
#include "maptile.h"
#include "mapselectrectangle.h"
#include "RXIO/RXObjects/rpgmap.h"
#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgtileset.h"
#include "RXIO/RXObjects/parserexception.h"

#include "event_dialogs/eventdialog.h"

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

    //connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(prepare_context_menu(QPoint)));

}

void MapView::set_mode(int mode)
{
    if (map == 0) return;

    if (this->opt.mode == SELECT && mode != SELECT && this->select_rectangle != 0)
    {
        //restore select rectangle to map
        this->select_rectangle->save_to_map(this->map);
    }
    this->opt.mode = mode;
    this->redraw();
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
    if (this->opt.mode != PEN)
        this->set_mode(PEN);
    this->brush = data;
    this->rectangle_offset = QPoint(0,0);
    this->rectangle->setSize(data.at(0), data.at(1));
    this->rectangle->update();
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    if (map == 0 || this->rectangle == 0) return;
    if (mapToScene(event->pos()).x() <= 0 || mapToScene(event->pos()).x() >= 32*map->width) return;
    if (mapToScene(event->pos()).y() <= 0 || mapToScene(event->pos()).y() >= 32*map->height) return;

    QPointF posf = mapToScene(event->pos());
    QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);
    if (pos == last_pos) return;
    this->last_pos = pos;

    emit mouse_over_coordinates(pos.x(), pos.y());

    if (opt.mode == PEN)
    {
        /*
        //workaround that rectangle does not go over topleft
        if ((pos+rectangle_offset).x() < 0)  pos.setX(-rectangle_offset.x());
        if ((pos+rectangle_offset).y() < 0)  pos.setY(-rectangle_offset.y());
        if ((pos+rectangle_offset).x()+rectangle->width() >= map->width)  pos.setX(map->width-rectangle_offset.x()-rectangle->width());
        if ((pos+rectangle_offset).y()+rectangle->height() >= map->height)  pos.setY(map->height-rectangle_offset.y()-rectangle->height());
        */

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
            last_valid_pos_in_draw_rectangle = pos;
            pos = topleft; //overwrite pos to topleft. rectangle should not move while rightclick
        }

        QPointF oldpos = rectangle->pos();
        rectangle->setPos(32*(pos+rectangle_offset));
        rectangle->update();
        //updates old position of rectangle (for scrolling)
        this->scene()->update(oldpos.x(), oldpos.y(),rectangle->boundingRect().width(), rectangle->boundingRect().height());
    }
    else if (opt.mode == SELECT)
    {
        if (this->select_rectangle == 0) return;
        /*
        if ((pos+select_rectangle_offset).x() < 0)  pos.setX(-select_rectangle_offset.x());
        if ((pos+select_rectangle_offset).y() < 0)  pos.setY(-select_rectangle_offset.y());
        if ((pos+select_rectangle_offset).x()+select_rectangle->width() >= map->width )  pos.setX(map->width-select_rectangle_offset.x()-select_rectangle->width());
        if ((pos+select_rectangle_offset).y()+select_rectangle->height()>= map->height)  pos.setY(map->height-select_rectangle_offset.y()-select_rectangle->height());
        */

        if (pos != select_click && !is_moving && this->select_mouse_button_pressed)
        {
            QPoint topleft = QPoint(qMin(pos.x(),this->select_click.x()),qMin(pos.y(),this->select_click.y()));
            this->select_rectangle->setSize(1+qAbs(pos.x()-this->select_click.x()),1+qAbs(pos.y()-this->select_click.y()));
            this->select_rectangle->setPos(32*topleft);
            this->select_rectangle->update();
        }
        else if (pos != select_click && is_moving && this->select_mouse_button_pressed)
        {
            this->select_rectangle->setPos(32*(pos+select_rectangle_offset));
            this->select_rectangle->update();
        }
    }
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if (map == 0 || this->rectangle == 0)
        return;

    QPointF posf = mapToScene(event->pos());
    QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);


    if (opt.mode == PEN)
    {

        if (event->button() == Qt::LeftButton)
        {
            /*
            //workaround that rectangle does not go over topleft
            if ((pos+rectangle_offset).x() < 0) pos.setX(-rectangle_offset.x());
            if ((pos+rectangle_offset).y() < 0) pos.setY(-rectangle_offset.y());
            if ((pos+rectangle_offset).x()+rectangle->width() >= map->width)  pos.setX(map->width-rectangle_offset.x()-rectangle->width());
            if ((pos+rectangle_offset).y()+rectangle->height() >= map->height)  pos.setY(map->height-rectangle_offset.y()-rectangle->height());
            */

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
            last_valid_pos_in_draw_rectangle = pos;
            this->set_brush(this->map->get_elements_in_rectangle(QRect(pos.x(),pos.y(),1,1),this->opt.layer,this->opt.layer));
            this->rectangle->update();
        }
    }
    else if (opt.mode == SELECT)
    {
        this->select_mouse_button_pressed = true;

        if (this->select_rectangle != 0 && this->select_rectangle->pos_is_in_rectangle(pos))
        {
            //allow move rectangle
            this->is_moving = true;
            this->select_rectangle_offset = QPoint(this->select_rectangle->x()/32, this->select_rectangle->y()/32) - pos;
        }
        else if (this->select_rectangle != 0)
        {
            //kill rectangle
            this->select_rectangle->save_to_map(this->map);
            this->kill_select_rectangle();
        }

        if (this->select_rectangle == 0)
        {
            //create rectangle
            this->select_click = pos;
            this->select_rectangle = new MapSelectRectangle(0,0);
            this->scene()->addItem(this->select_rectangle);
        }
    }
    else if (opt.mode == EVENT)
    {
        if (event->button() == Qt::LeftButton)
        {
            this->event_left_button_for_moving = true;
            this->event_for_moving = this->map->event_on_pos(pos);

            if (event_for_moving != 0)
            {
                //update old pos
                if (this->scene()->itemAt(this->opt.marked_event, QTransform()) != 0) this->scene()->update(); //itemAt(this->opt.marked_event, QTransform())->update();
                this->opt.marked_event = pos;
                //update new pos
                if (this->scene()->itemAt(this->opt.marked_event, QTransform()) != 0) this->scene()->update(); //itemAt(this->opt.marked_event, QTransform())->update();
            }
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
        if (this->rectangle != 0)
            this->rectangle_offset = QPoint(this->rectangle->pos().x()/32,this->rectangle->pos().y()/32)-last_valid_pos_in_draw_rectangle;

        if (this->brush.at(0) == 1 && this->brush.at(1) == 1 && this->brush.length() == 3)
            emit one_tile_selected(this->brush.at(2));
    }

    if (opt.mode == SELECT)
    {
        this->select_mouse_button_pressed = false;

        if (!is_moving)
        {
            //selection rectangle
            QPoint topleft = QPoint(this->select_rectangle->x()/32, this->select_rectangle->y()/32);

            this->select_rectangle->set_brush(this->map->get_elements_in_rectangle(QRect(topleft,this->select_rectangle->size()),0,2), this->tileset, &this->opt);
            this->select_rectangle->update();
            this->map->delete_elements_in_rectangle(QRect(topleft,this->select_rectangle->size()),0,2);
        }
        this->is_moving = false;
    }

    if (opt.mode == EVENT)
    {
        if (event->button() == Qt::LeftButton)
        {
            this->event_left_button_for_moving = false;
            if (this->event_for_moving != 0 && this->map->event_on_pos(pos) == 0) // dest pos is free
            {
                //QPointF oldf = mapToScene(32*QPoint(this->event_for_moving->x, this->event_for_moving->y));
                QPoint old = QPoint(this->event_for_moving->x, this->event_for_moving->y); //QPoint((int)oldf.x()/32, (int)oldf.y()/32);
                this->event_for_moving->x = pos.x();
                this->event_for_moving->y = pos.y();
                ((MapTile*)this->scene()->itemAt(32*old,QTransform()))->update();
                ((MapTile*)this->scene()->itemAt(32*pos,QTransform()))->update();
            }

        }
    }
}

void MapView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPointF posf = mapToScene(event->pos());
    QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);

    if (opt.mode == EVENT)
    {
        RPGEvent *event = this->map->event_on_pos(pos);
        if (event != 0)
        {
            EventDialog *dialog = new EventDialog(event,system);
            dialog->show();
        }
    }
}

void MapView::wheelEvent(QWheelEvent *event)
{
    if ((event->modifiers() & Qt::Modifier::CTRL) != 0)
    {
        if (event->pixelDelta().y() > 0)
            emit zoom_in();
        else emit zoom_out();

        return;
    }
    event->accept();
    QGraphicsView::wheelEvent(event);
}

void MapView::redraw()
{
    if (this->select_rectangle != 0)
    {
        delete this->select_rectangle;
        this->select_rectangle = 0;
    }
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
    this->scene()->setSceneRect(0,0,32*map->width,32*map->height);

    if (opt.mode == PEN)
    {
        this->rectangle_offset = QPoint(0,0);
        int w = 1;
        int h = 1;
        if (brush.length() > 2)
        {
            w = brush.at(0);
            h = brush.at(1);
        }
        this->rectangle = new MapRectangle(w,h);
        this->scene()->addItem(this->rectangle);
    }
    else if (opt.mode == SELECT)
    {
        //this->select_rectangle = new MapSelectRectangle(0,0); //not displayed
    }
}
