#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent)
{
    this->setMouseTracking(true);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(prepare_context_menu(QPoint)));
    this->height = 30;
    this->width = 60;
    this->current_layer = 0;
    this->mode = PEN;
    //this->map_values = (int*)calloc(this->height * this->width * 3, sizeof(int));
    for (int i = 0; i < height * width * 3; i++)
        this->map_values.append(0);
    this->brush_rectangle.setRect(0,0,1,1);

    this->redraw();
}

MapWidget::~MapWidget()
{
    //if(map_values != 0)
     //   free(map_values);
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
                    map_values[index] = brush_vars[shift_x + brush_rectangle.width()*shift_y];
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
    if (this->mode == PEN)
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
                    map_values[index] = brush_vars[x + brush_rectangle.width()*y];
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

    if (this->mode == PEN)
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
                    list.append(this->map_values[index]);
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
                        this->selection_list.append(map_values[index]);
                        map_values[index] = 0;
                    }
            selection_rectangle_is_released = true; //draw content of sel. rect now
        }
        this->redraw();
    }
}

int MapWidget::array_position(QPoint p, int layer)
{
    return p.x() + p.y() * this->width + this->height * this->width * layer;
}

void MapWidget::set_layer(int layer)
{
    this->current_layer = layer;
    this->redraw();
}

void MapWidget::set_dim(bool dim)
{
    this->dim_other_layers = dim;
    this->redraw();
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
    this->current_pic = QImage(this->width*32, this->height*32,QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&this->current_pic);
    painter.fillRect(0,0,this->current_pic.width(), this->current_pic.height(),Qt::black);


    for (int layer = 0; layer <= this->current_layer; layer++)
    {
        painter.setOpacity(1);
        if (this->dim_other_layers == true && layer < this->current_layer)
        {
            painter.setOpacity(0.5);
        }
        for (int i = 0; i < this->height * this->width; i++)
        {
            int index = layer * this->height * this->width + i;
            if (this->map_values[index] == 0)
            {
                //empty, do nothing
            }
            else if (this->map_values[index] > 0 && this->map_values[index] < 0x0180)
            {
                //autotiles
                int tileset_num = this->map_values[index] / 48 - 1;
                int tile_num = this->map_values[index] % 48;

                QPoint map_coord = 32*QPoint(i % this->width, i/this->width);
                QRect target_rect(map_coord, map_coord + QPoint(31,31));

                painter.drawImage(target_rect,this->map->tileset->autotiles[tileset_num].tileset_full,QRect((tile_num%8)*32,(tile_num/8)*32,32,32));


            }
            else{
                QPoint tile_coord = 32*bin_to_coordinate(map_values[index]);
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

                painter.drawImage(target_rect, *img, source_rect);

            }
        }

    }


    painter.end();
    this->setPixmap(QPixmap::fromImage(this->current_pic));
    if (this->mode == PEN)
        this->draw_brush_rectangle();
    else if (this->mode == SELECT)
        this->draw_selection_rectangle();
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
                map_values[index] = this->selection_list[2 + sel_width*sel_height*z + x + sel_width*y];
            }
}

void MapWidget::flood_fill(QPoint clicked, QPoint next)
{
    int shift_x = (clicked.x() > next.x() ? shitty_mod(clicked.x() - next.x(),this->brush_rectangle.width()) : ((next.x() - clicked.x())% this->brush_rectangle.width()));
    int shift_y = (clicked.y() > next.y() ? shitty_mod(clicked.y() - next.y(),this->brush_rectangle.height()) : ((next.y() - clicked.y())% this->brush_rectangle.height()));



    int index = array_position(next, this->current_layer);
    if (map_values[index] == 0)
    {
        map_values[index] = this->brush_vars[brush_rectangle.width()*brush_rectangle.height()*current_layer + shift_x + this->brush_rectangle.width()*shift_y];
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
    this->map_values = map->data;
    this->img = &map->tileset->tileset;

    /*
    this->img = QImage(this->current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() + map->tileset->tileset_name + ".png");
    if (this->img.isNull())
    {
        this->img = QImage(this->current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() + map->tileset->tileset_name + ".PNG");
        //shitty windows workaround
    }

    if (this->img.isNull())
    {
        qDebug() << this->current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() + map->tileset->tileset_name + ".png";
        exit(1);
    }
    */

    this->redraw();
}

void MapWidget::prepare_context_menu(QPoint p)
{

    //preparation for event stuff.
}
