#include "mapview.h"
#include "maprectangle.h"
#include "maptile.h"
#include "mapselectrectangle.h"
#include "events/eventdialog.h"


#include "RXIO2/rpgdb.h"

MapView::MapView(QWidget *parent) : QGraphicsView(parent)
{
    this->setScene(new QGraphicsScene);
    this->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    this->opt.layer = 0;
    this->opt.current_and_below = false;
    //this->opt.dim = true;
    this->opt.mode = PEN;

    this->action_newevent.setText("New Event");
    this->action_newevent.setShortcut(Qt::Key_Enter);
    connect(&this->action_newevent, &QAction::triggered, [=]{ if (opt.mode == EVENT) this->edit_event_on_pos(opt.marked_tile);});
    this->action_editevent.setText("Edit Event");
    this->action_editevent.setShortcut(Qt::Key_Return);
    connect(&this->action_editevent, &QAction::triggered, [=]{ if (opt.mode == EVENT) this->edit_event_on_pos(opt.marked_tile);});
    this->addAction(&this->action_editevent); //return should work
    this->action_cut.setText("Cut");
    this->action_cut.setShortcut(QKeySequence(tr("Ctrl+X")));
    connect(&this->action_cut, SIGNAL(triggered()), this, SLOT(do_cut()));
    this->action_copy.setText("Copy");
    this->action_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    connect(&this->action_copy, SIGNAL(triggered()), this, SLOT(do_copy()));
    this->action_paste.setText("Paste");
    this->action_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    connect(&this->action_paste, SIGNAL(triggered()), this, SLOT(do_paste()));
    this->action_delete.setText("Delete");
    this->action_delete.setShortcut(Qt::Key_Delete);
    connect(&this->action_delete, SIGNAL(triggered()), this, SLOT(do_delete()));
    this->action_set_start.setText("Set Starting Pos");
    this->action_follow_teleport.setText("Follow Teleport");
    this->action_follow_teleport.setShortcut(Qt::Key_F12);
    this->addAction(&this->action_follow_teleport);
    connect(&this->action_follow_teleport, &QAction::triggered, [=]()
    {
        QJsonObject event = this->mc.event_on_pos(opt.marked_tile);
        for (int p = 0; p < event.value("@pages").toArray().count(); p++)
        {
            for (int c = 0; c < event.value("@pages").toArray().at(p).toObject().value("@list").toArray().count(); c++)
            {
                if (event.value("@pages").toArray().at(p).toObject().value("@list").toArray().at(c).toObject().value("@code").toInt() == 201
                        && event.value("@pages").toArray().at(p).toObject().value("@list").toArray().at(c).toObject().value("@parameters").toArray().at(0).toInt() == 0)
                {
                    //qDebug() << "Page" << p+1 << "Line" << c << event.value("@pages").toArray().at(p).toObject().value("@list").toArray().at(c).toObject().value("@parameters").toArray();
                    emit jump_to_map(event.value("@pages").toArray().at(p).toObject().value("@list").toArray().at(c).toObject().value("@parameters").toArray().at(1).toInt());
                    return;
                }
            }
        }
    });

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(prepare_context_menu(QPoint)));

}

void MapView::set_mode(int mode)
{
    if (this->opt.mode == SELECT && mode != SELECT && this->select_rectangle != 0)
    {
        //restore select rectangle to map
        this->select_rectangle->save_to_map();
    }
    this->opt.mode = mode;
    this->redraw();
}

void MapView::set_map(int id)
{
    //do not delete selection when changing map
    if (this->opt.mode == SELECT && this->select_rectangle != 0)
    {
        //restore select rectangle to map
        this->select_rectangle->save_to_map();
    }
    this->mc.setMap(id);
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
    if (data.length() > 2)
        this->rectangle->setSize(data.at(0), data.at(1));
    this->rectangle->update();
}

void MapView::edit_event_on_pos(QPoint pos)
{
    QJsonObject event = this->mc.event_on_pos(pos);
    if (event.keys().count() > 1)
    {
        EventDialog *dialog = new EventDialog(&this->mc, event);
        dialog->show();
    }
    else
    {
        QJsonObject event_to_create = Factory().create_event(mc.get_next_event_id(),pos.x(),pos.y());
        EventDialog *dialog = new EventDialog(&this->mc, event_to_create); //create event
        dialog->show();

        connect(dialog, &EventDialog::ok_clicked, [=](){ //show created event
            this->undo.push(mc.get_current_map_id(),mc.current_map()->object());
            MapTile *tile;
            if ( (tile = ((MapTile*)this->scene()->itemAt(32*opt.marked_tile,QTransform()))) != 0)
                tile->update();
        });
    }
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    if (this->rectangle == 0) return;
    if (mapToScene(event->pos()).x() <= 0 || mapToScene(event->pos()).x() >= 32*mc.get_width()) return;
    if (mapToScene(event->pos()).y() <= 0 || mapToScene(event->pos()).y() >= 32*mc.get_height()) return;

    QPointF posf = mapToScene(event->pos());
    QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);
    if (pos == last_pos) return;
    this->last_pos = pos;

    emit mouse_over_coordinates(pos.x(), pos.y());

    if (opt.mode == PEN)
    {
        if (this->left_button_clicked)
        {
            QPoint rel_pos = pos - this->draw_from_pos;
            this->mc.put_elements_from_list(pos+rectangle_offset, rel_pos, this->brush, this->opt.layer, this->opt.layer);
        }
        else if (this->right_button_clicked)
        {
            QPoint topleft = QPoint(qMin(pos.x(),this->rightclick_from_pos.x()),qMin(pos.y(),this->rightclick_from_pos.y()));
            QPoint bottomright = QPoint(qMax(pos.x(),this->rightclick_from_pos.x()),qMax(pos.y(),this->rightclick_from_pos.y()));
            this->set_brush(this->mc.get_elements_in_rectangle(QRect(topleft,bottomright),opt.layer,opt.layer));
            last_valid_pos_in_draw_rectangle = pos;
            pos = topleft; //overwrite pos to topleft. rectangle should not move while rightclick
        }

        QPointF oldpos = rectangle->pos();
        rectangle->setPos(32*(pos+rectangle_offset));
        rectangle->update();
        //updates old position of rectangle (for scrolling)
        //-32 and +64 for autotiles (which updates tiles next to the current)
        this->scene()->update(oldpos.x()-32, oldpos.y()-32,rectangle->boundingRect().width()+64, rectangle->boundingRect().height()+64);
    }
    else if (opt.mode == SELECT)
    {
        if (this->select_rectangle == 0) return;

        if (!is_moving && this->select_mouse_button_pressed)
        {
            QPoint topleft = QPoint(qMin(pos.x(),this->select_click.x()),qMin(pos.y(),this->select_click.y()));
            this->select_rectangle->setSize(1+qAbs(pos.x()-this->select_click.x()),1+qAbs(pos.y()-this->select_click.y()));
            this->select_rectangle->setPos(32*topleft);
            this->select_rectangle->update();
        }
        else if (is_moving && this->select_mouse_button_pressed)
        {
            this->select_rectangle->setPos(32*(pos+select_rectangle_offset));
            this->select_rectangle->update();
        }
    }
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    QPointF posf = mapToScene(event->pos());
    QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);

    if (opt.mode == PEN)
    {
        if (this->rectangle == 0)
            return;
        if (event->button() == Qt::LeftButton)
        {
            this->left_button_clicked = true;
            this->draw_from_pos = pos;
            this->undo.push(mc.get_current_map_id(),mc.current_map()->object());
            this->mc.put_elements_from_list(pos+rectangle_offset, QPoint(0,0), this->brush, this->opt.layer, this->opt.layer);
            this->rectangle->update();
        }
        else if (event->button() == Qt::RightButton)
        {
            this->rectangle->setPos(32*pos);
            this->right_button_clicked = true;
            this->rightclick_from_pos = pos;
            last_valid_pos_in_draw_rectangle = pos;
            this->set_brush(this->mc.get_elements_in_rectangle(QRect(pos.x(),pos.y(),1,1),this->opt.layer,this->opt.layer));
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
            this->undo.push(mc.get_current_map_id(),mc.current_map()->object());
            this->select_rectangle->save_to_map();
            this->kill_select_rectangle();
        }

        if (this->select_rectangle == 0)
        {
            //create rectangle
            this->select_click = pos;
            this->select_rectangle = new MapSelectRectangle(&this->mc,0,0);
            this->scene()->addItem(this->select_rectangle);
        }
    }
    else if (opt.mode == EVENT)
    {
        //marked tile
        MapTile *tile;
        QPoint prev_marked_tile = opt.marked_tile;
        this->opt.marked_tile = pos;
        if ( (tile = ((MapTile*)this->scene()->itemAt(32*pos,QTransform()))) != 0)
            tile->update();
        if ( (tile = ((MapTile*)this->scene()->itemAt(32*prev_marked_tile,QTransform()))) != 0)
            tile->update();

        if (event->button() == Qt::LeftButton)
        {
            QJsonObject event = this->mc.event_on_pos(pos);
            if (event.contains("RXClass"))
            {
                this->event_left_button_for_moving = true;
                this->event_moving_from_pos.setX(event.value("@x").toInt());
                this->event_moving_from_pos.setY(event.value("@y").toInt());
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

    if (opt.mode == PEN && event->button() == Qt::RightButton)
    {
        this->right_button_clicked = false;
        if (this->rectangle != 0)
            this->rectangle_offset = QPoint(this->rectangle->pos().x()/32,this->rectangle->pos().y()/32)-last_valid_pos_in_draw_rectangle;

        if (this->brush.length() > 2 && this->brush.at(0) == 1 && this->brush.at(1) == 1 && this->brush.length() == 3)
            emit one_tile_selected(this->brush.at(2));
    }
    else if (opt.mode == SELECT)
    {
        this->select_mouse_button_pressed = false;

        if (!is_moving)
        {
            //selection rectangle
            QPoint topleft = QPoint(this->select_rectangle->x()/32, this->select_rectangle->y()/32);

            this->select_rectangle->set_brush(this->mc.get_elements_in_rectangle(QRect(topleft,this->select_rectangle->size()),0,2), &this->opt);
            this->select_rectangle->update();
            this->mc.delete_elements_in_rectangle(QRect(topleft,this->select_rectangle->size()),0,2);
        }
        this->is_moving = false;
    }

    if (opt.mode == EVENT)
    {
        if (event->button() == Qt::LeftButton && event_left_button_for_moving && this->event_moving_from_pos != pos)
        {
            QJsonObject obj = mc.current_map()->object(); //copy object before move
            if (this->mc.move_event(this->event_moving_from_pos, pos))
                this->undo.push(mc.get_current_map_id(), obj); //backup old obj
            MapTile *tile;
            if ( (tile = ((MapTile*)this->scene()->itemAt(32*event_moving_from_pos,QTransform()))) != 0)
                tile->update();
            if ( (tile = ((MapTile*)this->scene()->itemAt(32*pos,QTransform()))) != 0)
                tile->update();

            //marked tile
            QPoint prev_marked_tile = opt.marked_tile;
            this->opt.marked_tile = pos;
            if ( (tile = ((MapTile*)this->scene()->itemAt(32*prev_marked_tile,QTransform()))) != 0)
                tile->update();
        }
        this->event_left_button_for_moving = false;
    }
}

void MapView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPointF posf = mapToScene(event->pos());
    QPoint pos = QPoint((int)posf.x()/32, (int)posf.y()/32);

    if (opt.mode == EVENT)
    {
        this->edit_event_on_pos(pos);
    }
}

void MapView::wheelEvent(QWheelEvent *event)
{
    if ((event->modifiers() & Qt::Modifier::CTRL) != 0)
    {
        if (event->angleDelta().y() > 0)
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

    for (int y = 0; y < mc.get_height(); y++)
    {
        for (int x = 0; x < mc.get_width(); x++)
        {
            MapTile *tile = new MapTile(&this->mc, QPoint(x,y), &opt);
            tile->setPos(QPoint(32*x,32*y));
            this->scene()->addItem(tile);
        }
    }
    this->scene()->setSceneRect(0,0,32*mc.get_width(),32*mc.get_height());

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
}
