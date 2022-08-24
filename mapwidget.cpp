#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent)
{
    this->setParent(parent);
    this->setMouseTracking(true);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(prepare_context_menu(QPoint)));
    this->current_layer = 0;
    this->mode = PEN;



    this->event_move_from_pos = QPoint(-1,-1);



    this->action_new.setText("&New Event");
    this->action_new.setShortcut(QKeySequence(Qt::Key_Enter));
    this->addAction(&action_new);
    this->event_menu_with_new.addAction(&this->action_new);


    this->action_edit.setText("&Edit Event");
    this->action_edit.setShortcut(QKeySequence(Qt::Key_Enter));
    this->addAction(&action_edit);
    this->event_menu_with_edit.addAction(&this->action_edit);
    connect(&this->action_edit, SIGNAL(triggered()), this, SLOT(show_event_dialog()));


    this->event_menu_with_new.addSeparator();
    this->event_menu_with_edit.addSeparator();

    this->action_cut.setText("&Cut");
    this->action_cut.setShortcut(QKeySequence(tr("Ctrl+X")));
    this->event_menu_with_new.addAction(&this->action_cut);
    this->event_menu_with_edit.addAction(&this->action_cut);

    this->action_copy.setText("&Copy");
    this->action_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    this->event_menu_with_new.addAction(&this->action_copy);
    this->event_menu_with_edit.addAction(&this->action_copy);

    this->action_paste.setText("&Paste");
    this->action_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    this->event_menu_with_new.addAction(&this->action_paste);
    this->event_menu_with_edit.addAction(&this->action_paste);


    this->action_delete.setText("&Delete");
    this->action_delete.setShortcut(QKeySequence(Qt::Key_Delete));
    this->event_menu_with_new.addAction(&this->action_delete);
    this->event_menu_with_edit.addAction(&this->action_delete);



    this->event_menu_with_new.addSeparator();
    this->event_menu_with_edit.addSeparator();


    this->action_player_starting_pos.setText("&Set Player's starting Pos");
    this->event_menu_with_new.addAction(&this->action_player_starting_pos);
    this->event_menu_with_edit.addAction(&this->action_player_starting_pos);


    this->redraw();
}

MapWidget::~MapWidget()
{
}

void MapWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if (curr_pos.x() == ev->x()/tile && curr_pos.y() == ev->y()/tile)
        return; //only do this function when tile changed //cpu save
    if (ev->x() < 0 || ev->y() < 0)
        return; //out of range
    if (ev->x()/tile >= this->width || ev->y()/tile >= this->height)
        return; //out of range

    this->curr_pos = QPoint(ev->pos().x()/tile,ev->pos().y()/tile);

    if ( (this->mode == PEN && mouse_pressed_right))
    {
        this->rectangle_point1 = this->curr_pos;
        this->draw_rectangle();
    }
    else if (this->mode == PEN && !mouse_pressed_right)
    {
        // move pen rectangle
        QPoint diff = this->rectangle_point2-this->rectangle_point1;
        this->rectangle_point1 = this->curr_pos;
        this->rectangle_point2 = this->rectangle_point1+diff;

        if (mouse_pressed_left)
        {
            QPoint rel_pos = this->curr_pos - this->draw_from_position;
            map->put_elements_from_list(get_rectangle().topLeft(),rel_pos,brush,current_layer,current_layer);
            this->changes_made = true;
            this->redraw();
        }

        this->draw_rectangle();
    }
    else if (this->mode == SELECT && mouse_pressed_left)
    {
        this->select_rectangle_visible = true;
        if (select_rectangle_move)
        {
            QPoint diff = this->rectangle_point2-this->rectangle_point1;
            this->rectangle_point1 = this->select_rectangle_move_diff + this->curr_pos;
            this->rectangle_point2 = this->rectangle_point1 + diff;
            this->select_rectangle_tmp2 = get_rectangle();
            this->changes_made = true;
        }
        else
            this->rectangle_point1 = this->curr_pos;
        this->draw_rectangle();
    }


}

void MapWidget::mousePressEvent(QMouseEvent *ev)
{
    if (ev->x() < 0 || ev->y() < 0)
        return; //out of range
    if (ev->x()/tile >= this->width || ev->y()/tile >= this->height)
        return; //out of range

    this->curr_pos = QPoint(ev->pos().x()/tile,ev->pos().y()/tile);

    if (ev->button() == Qt::RightButton)
        this->mouse_pressed_right = true;
    else if(ev->button() == Qt::LeftButton)
        this->mouse_pressed_left = true;


    if (this->mode == EVENT && ev->button() == Qt::LeftButton)
    {
        this->event_move_from_pos = this->curr_pos;
        return;
    }
    else if (this->mode == PEN && this->mouse_pressed_right)
    {
        this->rectangle_point2 = this->curr_pos;
        this->draw_rectangle();
    }
    else if (this->mode == PEN && this->mouse_pressed_left)
    {
        this->draw_from_position = curr_pos;
        map->put_elements_from_list(get_rectangle().topLeft(),QPoint(0,0),brush,current_layer,current_layer);
        this->changes_made = true;
        this->redraw();
    }
    else if (this->mode == SELECT && this->mouse_pressed_left)
    {
        if (this->get_rectangle().contains(this->curr_pos))
        {
            //move it
            this->select_rectangle_move = true;
            this->select_rectangle_move_diff = this->rectangle_point1-this->curr_pos;
        }
        else if (!this->get_rectangle().contains(this->curr_pos) && this->select_rectangle_visible)
        {
            this->destroy_selection_rectangle();
            this->redraw();
        }
        else
        {
            this->destroy_selection_rectangle();
            this->redraw();
        }
    }
    else if (this->mode == FLOOD)
    {
        //this->flood_fill(this->curr_pos, this->curr_pos);
    }

    if (this->mode == SELECT || this->mode == PEN)
        this->draw_rectangle();
}

void MapWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    this->curr_pos = QPoint(ev->pos().x()/tile,ev->pos().y()/tile);

    if (ev->button() == Qt::RightButton)
        this->mouse_pressed_right = false;
    else if(ev->button() == Qt::LeftButton)
        this->mouse_pressed_left = false;



    if (this->mode == EVENT && ev->button() == Qt::LeftButton && event_move_from_pos != curr_pos) //move event
    {
        RPGEvent *event = map->event_on_pos(this->event_move_from_pos);
        if (event != 0)
        {
            event->x = curr_pos.x();
            event->y = curr_pos.y();
            this->changes_made = true;
            this->redraw();
        }
    }
    else if (this->mode == PEN && ev->button() == Qt::RightButton)
    {
        this->set_brush(map->get_elements_in_rectangle(get_rectangle(),current_layer,current_layer));
    }
    else if (this->mode == SELECT && ev->button() == Qt::LeftButton && select_rectangle_visible)
    {
        if (!select_rectangle_move)
        {
            this->select_rectangle_tmp1 = get_rectangle();
            this->select_rectangle_tmp2 = get_rectangle();
            this->selection_tmp = this->current_pic.copy(
                        QRect(this->select_rectangle_tmp1.topLeft()*tile, (this->select_rectangle_tmp1.bottomRight()+QPoint(1,1))*tile)
                        );
            this->redraw();
        }
    }


    if (this->select_rectangle_move && ev->button() == Qt::LeftButton)
        this->select_rectangle_move = false; //always false when mouse released
}

void MapWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if (this->mode == EVENT && ev->button() == Qt::LeftButton)
    {
        if (map->event_on_pos(this->curr_pos) != 0)
        {
            event_for_editing = map->event_on_pos(this->curr_pos);
            this->show_event_dialog();
        }
    }
}


void MapWidget::set_brush(QList<int> brush)
{
    this->brush = brush;
    if (get_rectangle().width() != brush.at(0) || get_rectangle().height() != brush.at(1))
    {
        //this should only happen when brush is set from external widget.
        //rectangle should not change when brush is selected at map
        this->rectangle_point1 = this->curr_pos;
        this->rectangle_point2 = this->rectangle_point1 + QPoint(brush.at(0)-1, brush.at(1)-1);
    }

    this->draw_rectangle();
}

void MapWidget::redraw()
{
    if (this->map == 0)
        return;

    this->current_pic = map->create_map_image(this->zoom,
                                              this->dim_other_layers,
                                              !this->show_all_layers,
                                              (this->mode==EVENT ? 3 : this->current_layer),
                                              this->system->tileset_hash.value(map->tileset_id)
                                              );

    this->draw_rectangle();
}

void MapWidget::draw_rectangle()
{
    QRect rect(this->get_rectangle().topLeft() * tile, (this->get_rectangle().bottomRight()+QPoint(1,1)) * tile);
    QImage newimg = QImage(current_pic);

    if (this->mode != SELECT || select_rectangle_visible)
    {
        QPainter painter;
        painter.begin(&newimg);
        if (this->mode == SELECT)
        {
            painter.setPen( QPen(QColor(0,0,0),3,Qt::DotLine));
            painter.drawRect(rect);
            painter.setPen( QPen(QColor(255,255,255),1,Qt::DotLine));
            painter.drawRect(rect);

            if (!this->selection_tmp.isNull())
            {
                painter.fillRect(QRect(this->select_rectangle_tmp1.topLeft()*tile, (QPoint(1,1) +this->select_rectangle_tmp1.bottomRight())*tile) , QColor(0xaa,0x16,0xa0));
                painter.drawImage(QRect(this->select_rectangle_tmp2.topLeft()*tile, (QPoint(1,1) +this->select_rectangle_tmp2.bottomRight())*tile), this->selection_tmp);
            }

        }
        else
        {
            painter.setPen( QPen(QColor(200,200,200),3));
            painter.drawRect(rect);
        }
        painter.end();
    }

    this->setPixmap(QPixmap::fromImage(newimg));
}



void MapWidget::prepare_context_menu(const QPoint &pos)
{
    if (this->mode == EVENT)
    {
        event_for_editing = map->event_on_pos(this->curr_pos);
        if (event_for_editing == 0)
            this->event_menu_with_new.exec(this->mapToGlobal(pos));
        else
            this->event_menu_with_edit.exec(this->mapToGlobal(pos));
    }
}

void MapWidget::show_event_dialog()
{
    EventDialog *dialog = new EventDialog(event_for_editing, this->system, 0);
    dialog->show();
}



QList<int> MapWidget::do_copy()
{
    if (this->mode == SELECT && select_rectangle_visible)
        return QList<int>(map->get_elements_in_rectangle(this->get_rectangle(),0,2));
    else
        return QList<int>();
}

QList<int> MapWidget::do_cut()
{
    QList<int> copied_data = do_copy();
    if (this->mode == SELECT && select_rectangle_visible)
    {
        map->delete_elements_in_rectangle(this->get_rectangle(),0,2);
        this->changes_made = true;
    }
    this->redraw();
    return copied_data;
}

void MapWidget::do_delete()
{
    if (this->mode == SELECT && select_rectangle_visible)
    {
        map->delete_elements_in_rectangle(this->get_rectangle(),0,2);
        this->changes_made = true;
    }
    this->redraw();
}

void MapWidget::do_paste(QList<int> data)
{
    this->changes_made = true;
}

void MapWidget::do_save()
{
    this->system->map_info_list.at(current_map_id)->save_map(this->system);
}

void MapWidget::do_withdraw()
{
    delete this->system->map_info_list.at(current_map_id)->map;
    this->system->map_info_list.at(current_map_id)->map = 0;
}


void MapWidget::set_mode(int mode)
{
    this->destroy_selection_rectangle();
    this->mode = mode;
    this->rectangle_point1 = this->rectangle_point2 = QPoint(0,0);
    this->redraw();
}

void MapWidget::destroy_selection_rectangle()
{
    if (!selection_tmp.isNull())
    {
        map->move_map_part(this->select_rectangle_tmp1, this->select_rectangle_tmp2.topLeft());
        this->changes_made = true;
    }
    this->selection_tmp = QImage();
    this->rectangle_point1 = this->curr_pos;
    this->rectangle_point2 = this->curr_pos;
    this->select_rectangle_visible = false;
}

void MapWidget::set_map(int map_id)
{
    this->current_map_id = map_id;
    this->destroy_selection_rectangle();
    this->system->map_info_list.at(map_id)->load_map(this->system);
    this->map = this->system->map_info_list.at(map_id)->map;

    if (!this->system->tileset_hash.contains(this->map->tileset_id))
    {
        //error tileset
        QMessageBox::critical(this,"Error", QString("Error: Tileset id  not found at predefined tilesets"));
        return; //
    }

    this->height = map->height;
    this->width = map->width;
    this->changes_made = false;

    this->redraw();
}
