#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent)
{
    this->setParent(parent);
    this->setMouseTracking(true);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(prepare_context_menu(QPoint)));
    this->height = 30;
    this->width = 60;
    this->current_layer = 0;
    this->mode = PEN;
    this->brush_rectangle.setRect(0,0,1,1);

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
    if (curr_pos.x() == ev->x()/32 && curr_pos.y() == ev->y()/32)
        return; //only do this function when tile changed //cpu save
    if (ev->x() < 0 || ev->y() < 0)
        return; //out of range
    if (ev->x()/32 >= this->width || ev->y()/32 >= this->height)
        return; //out of range

    this->curr_pos = QPoint(ev->pos().x()/32,ev->pos().y()/32);

    if (this->mode == PEN)
    {
        if (mouse_pressed_right) // Update brush rectangle
        {
            QPoint topleft(qMin(this->brush_first_click_pos.x(),this->curr_pos.x()),
                           qMin(this->brush_first_click_pos.y(),this->curr_pos.y()));
            QPoint bottomright(qMax(this->brush_first_click_pos.x(),this->curr_pos.x()),
                               qMax(this->brush_first_click_pos.y(),this->curr_pos.y()));
            this->brush_rectangle.setTopLeft(topleft);
            this->brush_rectangle.setBottomRight(bottomright);
        }
        else if (mouse_pressed_left) //draw brush content
        {
            QPoint draw_pos = this->curr_pos + brush_rectangle_mouse_alignment;
            for (int x = 0; x < this->brush_rectangle.width(); x++ )
            {
                if (draw_pos.x() + x >= this->width || draw_pos.x() + x < 0)
                    continue;
                for (int y = 0; y < this->brush_rectangle.height(); y++ )
                {
                    if (draw_pos.y() + y >= this->height || draw_pos.y() + y < 0)
                        continue;
                    int index = array_position(QPoint(draw_pos.x()+x,draw_pos.y()+y), this->current_layer);

                    int shift_x = (x + shitty_mod(left_click_pos.x()-curr_pos.x(),brush_rectangle.width()))%brush_rectangle.width();
                    int shift_y = (y + shitty_mod(left_click_pos.y()-curr_pos.y(),brush_rectangle.height()))%brush_rectangle.height();
                    this->map->data[index] = brush_vars[shift_x + brush_rectangle.width()*shift_y];
                }
            }
        }

        //brush follow mouse if right clicked
        if (!mouse_pressed_right)
        {
            this->brush_rectangle.moveTo(this->curr_pos + this->brush_rectangle_mouse_alignment);
        }

    } //end PEN
    else if (this->mode == SELECT)
    {
        if (this->selection_rectangle.width() > 0 && this->selection_rectangle.height() > 0)
        {
            if (this->selection_rectangle.contains(this->curr_pos)){
                this->setCursor(Qt::SizeAllCursor);
            }
            else
            {
                this->setCursor(QCursor());
            }
        }
        if (this->selection_button)
        {
            if (this->moving_selection) //move rectangle
            {
                //dont use moveTo 1-liner, because the rectangle can be "grabbed" on any tile
                QPoint moving_direction = this->curr_pos - this->move_pos_temp;
                this->move_pos_temp = this->curr_pos;
                this->selection_rectangle.setTopLeft(this->selection_rectangle.topLeft() + moving_direction);
                this->selection_rectangle.setBottomRight(this->selection_rectangle.bottomRight() + moving_direction);
            }
            else //update rectangle
            {
                QPoint topleft(qMin(this->selection_first_click_pos.x(),this->curr_pos.x()),
                               qMin(this->selection_first_click_pos.y(),this->curr_pos.y()));
                QPoint bottomright(qMax(this->selection_first_click_pos.x(),this->curr_pos.x()),
                                   qMax(this->selection_first_click_pos.y(),this->curr_pos.y()));
                this->selection_rectangle.setTopLeft(topleft);
                this->selection_rectangle.setBottomRight(bottomright);
                this->selection_rectangle_is_released = false; //don't draw content of rectangle while adjusting
            }
        }
    }

    this->redraw();
}

void MapWidget::mousePressEvent(QMouseEvent *ev)
{
    if (ev->x() < 0 || ev->y() < 0)
        return; //out of range
    if (ev->x()/32 >= this->width || ev->y()/32 >= this->height)
        return; //out of range

    this->curr_pos = QPoint(ev->pos().x()/32,ev->pos().y()/32);


    if (this->mode == EVENT)
    {
        if (ev->button() == Qt::LeftButton)
        {
            this->event_move_from_pos = this->curr_pos;
            return; // do nothing when leftclick //TODO: move events
        }
        if (ev->button() == Qt::RightButton)
        {
            //context menu
        }
    }
    else if (this->mode == PEN)
    {
        if (ev->button() == Qt::RightButton)
        {
            this->mouse_pressed_right = true;
            this->brush_rectangle = QRect(curr_pos.x(), curr_pos.y(),1,1);
            this->brush_first_click_pos = this->curr_pos;
        }
        else //Left mouse button
        {
            this->mouse_pressed_left = true;
            if (brush_vars.length() <= 0)
                return; //no brush, cant edit map

            this->left_click_pos = this->curr_pos;
            QPoint draw_pos = this->curr_pos + brush_rectangle_mouse_alignment;

            for (int x = 0; x < this->brush_rectangle.width(); x++ )
            {
                if (draw_pos.x() + x >= this->width || draw_pos.x() + x < 0)
                    continue;
                for (int y = 0; y < this->brush_rectangle.height(); y++ )
                {
                    if (draw_pos.y() + y >= this->height || draw_pos.y() + y < 0)
                        continue;
                    int index = array_position(QPoint(draw_pos.x()+x,draw_pos.y()+y), this->current_layer);
                    this->map->data[index] = brush_vars[x + brush_rectangle.width()*y];
                }
            }
        }
    } //end PEN
    else if (this->mode == SELECT)
    {
        this->selection_button = true;
        if (this->selection_rectangle.width() > 0 && this->selection_rectangle.height() > 0) //when selection rect exists
        {
            if (this->selection_rectangle.contains(this->curr_pos)) //click inside means moving
            {
                this->moving_selection = true;
                this->move_pos_temp = this->curr_pos;
            }
            else //click outside means clear rectangle
            {
                this->merge_selection();
                this->selection_rectangle = QRect(-1,-1,0,0);
                this->selection_first_click_pos = this->curr_pos; //pos for new rectangle (only if mouse moving)
            }
        }
        else //start new selection rectangle
        {
            this->selection_first_click_pos = this->curr_pos;
        }
     }
    else if (this->mode == FLOOD)
    {
        this->flood_fill(this->curr_pos, this->curr_pos);
    }


    this->redraw();
}

void MapWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    this->curr_pos = QPoint(ev->pos().x()/32,ev->pos().y()/32);

    if (this->mode == EVENT) //move event
    {
        if (ev->button() == Qt::LeftButton)
        {
            if (this->event_move_from_pos != QPoint(-1,-1))
            {
                int new_pos;
                bool move_allowed = true;
                for (int i = 0; i < this->map->events.length(); i++)
                {
                    if (this->map->events.at(i)->x == this->event_move_from_pos.x() && this->map->events.at(i)->y == this->event_move_from_pos.y())
                    {
                        new_pos = i;
                    }
                    if (this->map->events.at(i)->x == this->curr_pos.x() && this->map->events.at(i)->y == this->curr_pos.y())
                    {
                        move_allowed = false;
                    }
                }
                if (move_allowed)
                {
                    this->map->events.at(new_pos)->x = this->curr_pos.x();
                    this->map->events.at(new_pos)->y = this->curr_pos.y();
                    this->redraw();
                }
            }
        }
    }
    else if (this->mode == PEN)
    {
        if (ev->button() == Qt::RightButton) //new brush on right click
        {
            this->mouse_pressed_right = false;
            QList<int> list;
            list.append(brush_rectangle.width());
            list.append(brush_rectangle.height());

            int index;
            for (int y = 0; y < brush_rectangle.height(); y++)
            {
                for (int x = 0; x < brush_rectangle.width(); x++)
                {
                    index = array_position(brush_rectangle.topLeft() + QPoint(x,y), this->current_layer);
                    list.append(this->map->data[index]);
                }
            }
            this->set_brush(list);
            this->brush_rectangle_mouse_alignment = this->brush_rectangle.topLeft() - this->curr_pos;
        }
        else //left button
            this->mouse_pressed_left = false;
    } //end PEN
    else if (this->mode == SELECT)
    {
        this->selection_button = false;
        if (moving_selection)
        {
            this->moving_selection = false;
        }
        else
        {
            this->selection_list.clear();
            this->selection_list.append(this->selection_rectangle.width());
            this->selection_list.append(this->selection_rectangle.height());

            for (int z = 0; z < 3; z++)
                for (int y = 0; y < selection_rectangle.height()-1; y++)
                    for (int x = 0; x < selection_rectangle.width()-1; x++)
                    {
                        int index = array_position(selection_rectangle.topLeft() + QPoint(x,y), z);
                        this->selection_list.append(this->map->data[index]);
                        this->map->data[index] = 0;
                    }
            selection_rectangle_is_released = true; //draw content of sel. rect now
        }
        this->redraw();
    }
}





void MapWidget::set_brush(QList<int> vars)
{
    this->brush_rectangle.setWidth(vars.at(0));
    this->brush_rectangle.setHeight(vars.at(1));

    vars.pop_front();
    vars.pop_front();

    this->brush_vars = vars;
    this->brush_rectangle_mouse_alignment = QPoint(0,0);
}

void MapWidget::redraw()
{
    if (this->map == 0)
        return;
    this->current_pic = QImage(this->width*32, this->height*32,QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&this->current_pic);
    painter.fillRect(0,0,this->current_pic.width(), this->current_pic.height(), Qt::black); ;


    for (int layer = 0; layer <= (this->show_all_layers ? 2 : this->current_layer); layer++)
    {
        painter.setOpacity(1);
        if (this->dim_other_layers == true && layer < this->current_layer)
        {
            painter.setOpacity(0.5);
        }
        else if (this->show_all_layers == true && layer == this->current_layer)
        {
            //opacity already 1
        }
        else if (this->show_all_layers == true && layer != this->current_layer)
        {
            painter.setOpacity(0.5); //always transparent when show all layers (different to original)
        }

        if (this->mode == EVENT)
        {
            painter.setOpacity(1); //no transparent when event
        }

        for (int i = 0; i < this->height * this->width; i++)
        {
            int index = layer * this->height * this->width + i;
            if (this->map->data[index] == 0 && layer == 0)
            {
                //empty, do nothing
                QPoint map_coord = 32*QPoint(i % this->width, i/this->width);
                QRect target_rect(map_coord, map_coord + QPoint(31,31));
                painter.fillRect(target_rect, QColor(0xaa,0x16,0xa0));
            }
            else if (this->map->data[index] > 0 && this->map->data[index] < 0x0180)
            {
                //autotiles
                int tileset_num = this->map->data[index] / 48 - 1;
                int tile_num = this->map->data[index] % 48;

                QPoint map_coord = 32*QPoint(i % this->width, i/this->width);
                QRect target_rect(map_coord, map_coord + QPoint(31,31));

                RPGTileset *tileset = settings->tileset_hash.value(map->tileset_id);
                painter.drawImage(target_rect,tileset->autotiles[tileset_num].tileset_full,QRect((tile_num%8)*32,(tile_num/8)*32,32,32));


            }
            else{
                QPoint tile_coord = 32*bin_to_coordinate(this->map->data[index]);
                QPoint map_coord = 32*QPoint(i % this->width, i/this->width);

                QRect target_rect(map_coord, map_coord + QPoint(31,31));
                QRect source_rect(tile_coord, tile_coord + QPoint(31,31));

                painter.drawImage(target_rect, *img, source_rect);
            }
        }
        if (mode == SELECT && this->selection_rectangle.width() > 0 && this->selection_rectangle.height() > 0
                && selection_rectangle_is_released)
        {
            //insert selection
            int sel_width = this->selection_rectangle.width()-1;
            int sel_height = this->selection_rectangle.height()-1;

            for (int i = 0; i < sel_width*sel_height;i++)
            {
                QPoint map_coord = 32*(this->selection_rectangle.topLeft() + QPoint(i % sel_width, i / sel_width));
                QPoint tile_coord = 32*bin_to_coordinate(this->selection_list[2 + sel_width*sel_height*layer +i]);

                QRect target_rect(map_coord, map_coord + QPoint(31,31));
                QRect source_rect(tile_coord, tile_coord + QPoint(31,31));

                //TODO Autotiles
                painter.drawImage(target_rect, *img, source_rect);

            }
        }

    }


    if (this->mode == EVENT) //grid lines
    {
        QPen pen;
        pen.setWidth(2);
        pen.setColor(QColor(150,150,150));
        painter.setPen(pen);

        for (int y = 0; y < this->height; y++)
        {
            painter.drawLine(0,32*y,this->width*32,32*y);
        }
        for (int x = 0; x < this->width; x++)
        {
            painter.drawLine(32*x,0,32*x,this->height*32);
        }
    }


    painter.end();
    this->setPixmap(QPixmap::fromImage(this->current_pic));
    if (this->mode != EVENT)
    {
        if (this->mode == PEN)
            this->draw_brush_rectangle();
        else if (this->mode == SELECT)
            this->draw_selection_rectangle();
    }

    if (this->mode == EVENT)
    {
        this->draw_events();
    }
}

void MapWidget::draw_events()
{
    QImage newimg = QImage(current_pic);
    QPainter painter;
    painter.begin(&newimg);

    painter.setPen(Qt::white);
    for (int i = 0; i < map->events.length(); i++)
    {
        RPGEvent *event = map->events.at(i);

        painter.drawRect(32*event->x+1, 32*event->y+1, 30,30);
        painter.setOpacity(0.5);
        painter.fillRect(32*event->x+1, 32*event->y+1, 30,30, Qt::white);
        painter.setOpacity(1);
        if (event->pages.length() >= 1)
        {
            if (!event->pages.at(0)->graphic->graphics.isNull())
            {

                //direction:
                //2 down, 4 left, 6 right, 8 up

                //pattern 0,1,2,3

                int dir_offset = (event->pages.at(0)->graphic->direction / 2) - 1;
                int pattern_offset = event->pages.at(0)->graphic->pattern;

                int pic_height = event->pages.at(0)->graphic->graphics.size().height()/4; //persons have 48 pixels, pokeballs 32

                painter.drawImage(QRect(32*event->x, 32*event->y, 31,31),event->pages.at(0)->graphic->graphics,QRect(32*pattern_offset,pic_height*dir_offset,32,32));
            }
        }

    }

    painter.end();
    this->setPixmap(QPixmap::fromImage(newimg));
}

void MapWidget::draw_brush_rectangle()
{
    QImage newimg = QImage(current_pic);
    QPainter painter;
    painter.begin(&newimg);
    painter.setPen( QPen(QColor(200,200,200),3));
    painter.drawRect(QRect(this->brush_rectangle.topLeft()*32, (QPoint(1,1) +this->brush_rectangle.bottomRight())*32));
    painter.end();

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
    EventDialog *dialog = new EventDialog(event_for_editing, this->settings, 0);
    dialog->show();
}

void MapWidget::draw_selection_rectangle()
{
    if (this->selection_rectangle.width() > 0 && this->selection_rectangle.height() > 0)
    {
        QImage newimg = QImage(current_pic);
        QPainter painter;
        painter.begin(&newimg);
        painter.setPen( QPen(QColor(0,0,0),3,Qt::DotLine));
        painter.drawRect(QRect(this->selection_rectangle.topLeft()*32, this->selection_rectangle.bottomRight()*32));
        painter.setPen( QPen(QColor(255,255,255),1,Qt::DotLine));
        painter.drawRect(QRect(this->selection_rectangle.topLeft()*32, this->selection_rectangle.bottomRight()*32));
        painter.end();
        this->setPixmap(QPixmap::fromImage(newimg));
    }
}

QList<int> MapWidget::do_copy()
{
    return this->selection_list;
}

QList<int> MapWidget::do_cut()
{
    this->selection_rectangle = QRect(-1,-1,0,0);
    this->redraw();

    QList <int>list = QList<int>(this->selection_list);
    this->selection_list.clear();
    return list;
}

void MapWidget::do_delete()
{
    this->selection_rectangle = QRect(-1,-1,0,0);
    this->selection_list.clear();
    this->redraw();
}

void MapWidget::do_paste(QList<int> data)
{
    this->merge_selection();
    this->selection_list = data;
    this->selection_rectangle.setTopLeft(this->curr_pos);
    this->selection_rectangle.setWidth(data[0]);
    this->selection_rectangle.setHeight(data[1]);

    this->redraw();
}

void MapWidget::merge_selection()
{
    if (this->selection_rectangle.x() < 0 || this->selection_rectangle.y() < 0)
        return; //dont do merge if no selection rectangle is available

    int sel_height = this->selection_rectangle.height()-1;
    int sel_width = this->selection_rectangle.width()-1;

    for (int z = 0; z < 3; z++)
        for (int y = 0; y < selection_rectangle.height()-1; y++)
            for (int x = 0; x < selection_rectangle.width()-1; x++)
            {
                int index = array_position(selection_rectangle.topLeft() + QPoint(x,y), z);
                this->map->data[index] = this->selection_list[2 + sel_width*sel_height*z + x + sel_width*y];
            }
}

void MapWidget::flood_fill(QPoint clicked, QPoint next)
{
    int shift_x = (clicked.x() > next.x() ? shitty_mod(clicked.x() - next.x(),this->brush_rectangle.width()) : ((next.x() - clicked.x())% this->brush_rectangle.width()));
    int shift_y = (clicked.y() > next.y() ? shitty_mod(clicked.y() - next.y(),this->brush_rectangle.height()) : ((next.y() - clicked.y())% this->brush_rectangle.height()));



    int index = array_position(next, this->current_layer);
    if (this->map->data[index] == 0)
    {
        this->map->data[index] = this->brush_vars[brush_rectangle.width()*brush_rectangle.height()*current_layer + shift_x + this->brush_rectangle.width()*shift_y];
        if (next.x()-1 >= 0)
        {
            this->flood_fill(clicked, next + QPoint(-1,0));
        }
        if (next.y()-1 >= 0)
        {
            this->flood_fill(clicked, next + QPoint(0,-1));
        }
        if (next.x()+1 < this->width)
        {
            this->flood_fill(clicked, next + QPoint(1,0));
        }
        if (next.y()+1 < this->height)
        {
            this->flood_fill(clicked, next + QPoint(0,1));
        }

    }
}




int MapWidget::coordinate_to_bin(QPoint p)
{
    int val = 0x0180; //topleft value
    val += p.x();
    val += p.y() * 8;
    return val;
}

QPoint MapWidget::bin_to_coordinate(int b)
{
    b -= 0x0180; //remove offset
    return QPoint(b % 8, b/8);
}

void MapWidget::set_mode(int mode)
{
    this->mode = mode;
    this->selection_rectangle = QRect(-1,-1,0,0);
    this->redraw();
}

void MapWidget::set_map(RPGMap *map)
{
    this->map = map;
    this->height = map->height;
    this->width = map->width;
    this->img = &settings->tileset_hash.value(map->tileset_id)->tileset;

    this->redraw();
}
