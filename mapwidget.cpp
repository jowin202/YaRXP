#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent)
{
    this->setMouseTracking(true);
    this->height = 20;
    this->width = 32;
    this->current_layer = 0;
    this->mode = PEN;
    this->map_values = (int*)calloc(this->height * this->width * 3, sizeof(int));

    this->redraw();

}

MapWidget::~MapWidget()
{
    if(map_values != 0)
        free(map_values);
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
    this->draw_brush_rectangle();

    if (mouse_pressed_right)
    {
        //nothing happens here
    }
    else if (mouse_pressed_left)
    {
        for (int x = 0; x < this->brush_rectangle_x; x++ )
        {
            if (curr_pos.x() + x >= this->width)
                continue;
            for (int y = 0; y < this->brush_rectangle_y; y++ )
            {
                if (curr_pos.y() + y >= this->height)
                    continue;
                int index = array_position(QPoint(this->curr_pos.x()+x,this->curr_pos.y()+y), this->current_layer);

                int shift_x = (x + shitty_mod(left_click_pos.x()-curr_pos.x(),brush_rectangle_x))%brush_rectangle_x;
                int shift_y = (y + shitty_mod(left_click_pos.y()-curr_pos.y(),brush_rectangle_y))%brush_rectangle_y;
                map_values[index] = brush_vars[shift_x + brush_rectangle_x*shift_y];
            }
        }
        this->redraw();
    }


}

void MapWidget::mousePressEvent(QMouseEvent *ev)
{
    if (ev->x() < 0 || ev->y() < 0)
        return; //out of range
    if (ev->x()/32 >= this->width || ev->y()/32 >= this->height)
        return; //out of range

    this->curr_pos = QPoint(ev->pos().x()/32,ev->pos().y()/32);
    if (ev->button() == Qt::RightButton)
    {
        this->mouse_pressed_right = true;
        this->tmp_brush = QRect(curr_pos.x(), curr_pos.y(),1,1);
    }
    else //Left mouse button
    {
        this->mouse_pressed_left = true;
        if (brush_vars.length() <= 0)
            return; //no brush, cant edit map

        this->left_click_pos = this->curr_pos;

        for (int x = 0; x < this->brush_rectangle_x; x++ )
        {
            if (curr_pos.x() + x >= this->width)
                continue;
            for (int y = 0; y < this->brush_rectangle_y; y++ )
            {
                if (curr_pos.y() + y >= this->height)
                    continue;
                int index = array_position(QPoint(this->curr_pos.x()+x,this->curr_pos.y()+y), this->current_layer);
                map_values[index] = brush_vars[x + brush_rectangle_x*y];
            }
        }
    }

    this->redraw();
}

void MapWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    this->curr_pos = QPoint(ev->pos().x()/32,ev->pos().y()/32);

    if (ev->button() == Qt::RightButton) //new brush on right click
    {
        this->mouse_pressed_right = false;
        QList<int> list;
        list.append(tmp_brush.width());
        list.append(tmp_brush.height());

        int index;
        for (int y = 0; y < tmp_brush.height(); y++)
        {
            for (int x = 0; x < tmp_brush.width(); x++)
            {
                index = array_position(tmp_brush.topLeft() + QPoint(x,y), this->current_layer);
                list.append(this->map_values[index]);
            }
        }
        this->set_brush(list);
    }
    else //left button
        this->mouse_pressed_left = false;
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
    this->brush_rectangle_x = vars.at(0);
    this->brush_rectangle_y = vars.at(1);

    vars.pop_front();
    vars.pop_front();

    this->brush_vars = vars;
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
            if (this->map_values[index] != 0)
            {
                QPoint tile_coord = 32*bin_to_coordinate(map_values[index]);
                QPoint map_coord = 32*QPoint(i % this->width, i/this->width);

                QRect target_rect(map_coord, map_coord + QPoint(31,31));
                QRect source_rect(tile_coord, tile_coord + QPoint(31,31));

                painter.drawImage(target_rect, img2, source_rect);
            }
        }
    }


    painter.end();
    this->setPixmap(QPixmap::fromImage(this->current_pic));
    this->draw_brush_rectangle();
}

void MapWidget::draw_brush_rectangle()
{
    int rect_x;
    int rect_y;
    int rect_w;
    int rect_h;

    if (mouse_pressed_right)
    {
        rect_x = 32*tmp_brush.left();
        rect_y = 32*tmp_brush.top();
        rect_w = 32 * (1+curr_pos.x() - tmp_brush.left());
        rect_h = 32 * (1+curr_pos.y() - tmp_brush.top());
        this->tmp_brush.setSize(QSize((1+curr_pos.x() - tmp_brush.left()),(1+curr_pos.y() - tmp_brush.top())));
    }
    else{
        rect_x = this->curr_pos.x()*32;
        rect_y = this->curr_pos.y()*32;
        rect_w = this->brush_rectangle_x*32;
        rect_h = this->brush_rectangle_y*32;
    }


    QImage newimg = QImage(current_pic);
    QPainter painter;
    painter.begin(&newimg);
    painter.setPen( QPen(QColor(200,200,200),3));
    painter.drawRect(rect_x, rect_y, rect_w, rect_h);
    painter.end();

    this->setPixmap(QPixmap::fromImage(newimg));
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
}
