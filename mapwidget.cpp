#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent)
{
    this->setMouseTracking(true);
    this->height = 20;
    this->width = 20;
    this->current_layer = 0;
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
    this->curr_pos = QPoint(ev->pos().x()/32,ev->pos().y()/32);
    this->draw_selection_rectangle();

    if (mouse_pressed_right)
    {
        //this->selection_rectangle_x = qAbs(curr_pos.x() - tmp_point_selection_rectangle.x());
        //this->selection_rectangle_y = qAbs(curr_pos.y() - tmp_point_selection_rectangle.y());
    }

    if (mouse_pressed_left)
    {
        for (int x = 0; x < this->selection_rectangle_x; x++ )
        {
            for (int y = 0; y < this->selection_rectangle_y; y++ )
            {
                int index = array_position(QPoint(this->curr_pos.x()+x,this->curr_pos.y()+y), this->current_layer);
                map_values[index] = selection_vars[x + selection_rectangle_x*y];
            }
        }
        this->redraw();
    }


}

void MapWidget::mousePressEvent(QMouseEvent *ev)
{
    this->curr_pos = QPoint(ev->pos().x()/32,ev->pos().y()/32);
    if (ev->button() == Qt::RightButton)
    {
        this->mouse_pressed_right = true;
        //this->tmp_point_selection_rectangle = curr_pos;
        this->tmp_selection = QRect(curr_pos.x(), curr_pos.y(),1,1);
        /* //right click, only one tile
        QList<int> list;
        list.append(1);tmp_point_selection_rectangle.x(
        list.append(1);
        int index = array_position(curr_pos, this->current_layer);
        list.append(this->map_values[index]);
        this->set_selection(list);
        */
    }
    else //Left mouse button
    {
        this->mouse_pressed_left = true;
        if (selection_vars.length() <= 0)
            return; //no selection, cant edit map


        for (int x = 0; x < this->selection_rectangle_x; x++ )
        {
            for (int y = 0; y < this->selection_rectangle_y; y++ )
            {
                int index = array_position(QPoint(this->curr_pos.x()+x,this->curr_pos.y()+y), this->current_layer);
                map_values[index] = selection_vars[x + selection_rectangle_x*y];
            }
        }


        /*
        for (int i = 400; i < 800; i++)
        {
            if (i%20 == 0) std::cout << std::endl;
            std::cout << map_values[i] << " ";
        }
        std::cout << std::endl;
        */
    }

    this->redraw();
}

void MapWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    this->curr_pos = QPoint(ev->pos().x()/32,ev->pos().y()/32);

    if (ev->button() == Qt::RightButton)
    {
        qDebug() << tmp_selection;
        this->mouse_pressed_right = false;
        QList<int> list;
        list.append(tmp_selection.width());
        list.append(tmp_selection.height());

        int index;
        for (int y = 0; y < tmp_selection.height(); y++)
        {
            for (int x = 0; x < tmp_selection.width(); x++)
            {
                index = array_position(tmp_selection.topLeft() + QPoint(x,y), this->current_layer);
                list.append(this->map_values[index]);
            }
        }
        this->set_selection(list);
    }
    else
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

void MapWidget::set_selection(QList<int> vars)
{
    //qDebug() << "selection changed to MapWidget";
    //qDebug() << vars;
    this->selection_rectangle_x = vars.at(0);
    this->selection_rectangle_y = vars.at(1);

    vars.pop_front();
    vars.pop_front();

    this->selection_vars = vars;
}

void MapWidget::redraw()
{
    this->current_pic = QImage(this->width*32, this->height*32,QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&this->current_pic);
    painter.fillRect(0,0,this->current_pic.width(), this->current_pic.height(),Qt::black);

    /*
    for (int i = 0; i < this->height * this->width; i++)
    {
        if (this->map_values[i] != 0)
        {
            QPoint tile_coord = 32*bin_to_coordinate(map_values[i]);
            QPoint map_coord = 32*QPoint(i % this->width, i/this->width);

            QRect target_rect(map_coord, map_coord + QPoint(31,31));
            QRect source_rect(tile_coord, tile_coord + QPoint(31,31));

            painter.drawImage(target_rect, img2, source_rect);
        }
    }
    */


    for (int layer = 0; layer <= this->current_layer; layer++)
    {
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
    this->draw_selection_rectangle();
}

void MapWidget::draw_selection_rectangle()
{
    int rect_x;
    int rect_y;
    int rect_w;
    int rect_h;

    if (mouse_pressed_right)
    {
        rect_x = 32*tmp_selection.left();
        rect_y = 32*tmp_selection.top();
        rect_w = 32 * (1+curr_pos.x() - tmp_selection.left());
        rect_h = 32 * (1+curr_pos.y() - tmp_selection.top());
        this->tmp_selection.setSize(QSize((1+curr_pos.x() - tmp_selection.left()),(1+curr_pos.y() - tmp_selection.top())));
    }
    else{
        rect_x = this->curr_pos.x()*32;
        rect_y = this->curr_pos.y()*32;
        rect_w = this->selection_rectangle_x*32;
        rect_h = this->selection_rectangle_y*32;
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
