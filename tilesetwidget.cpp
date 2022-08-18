#include "tilesetwidget.h"

TilesetWidget::TilesetWidget(QWidget *parent)
{
    this->setMouseTracking(true);
    this->click_pos = QPoint(-1,-1);

    this->updateView();


}

void TilesetWidget::updateView()
{
    if (this->img == 0)
        return;

    if (range == 0) //autotiles only
    {
        QImage background(256,32, QImage::Format_ARGB32);

        QPainter painter(&background);
        painter.fillRect(0,0,background.width(),background.height(), QColor(0xaa,0x16,0xa0));

        if (this->current_tileset->autotiles.length() == 7)
        {
            for (int i = 0; i < 7; i++)
            painter.drawImage(QRect(32*(1+i),0,32,32), current_tileset->autotiles.at(i).thumb, QRect(0,0,32,32));
        }
        painter.end();

        this->current_image = background;
        this->setPixmap(QPixmap::fromImage(background));
    }
    else if (range == 1) //normal view, except if exceeding 32768
    {
        this->current_image = img->copy(0,0,img->width(),qMin(img->height(),32768));
        this->setPixmap(QPixmap::fromImage(current_image));
    }
    else if (range == 2) //only available if tileset > 32768
    {
        if (img->height() < 32768)
            return;
        this->current_image = img->copy(0,32768,img->width(),img->height()-32768);
        this->setPixmap(QPixmap::fromImage(current_image));
    }


}

void TilesetWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if (range == 0) return; //no need for autotiles

    if (ev->pos().x()/32 > 7 || ev->pos().x() < 0 || ev->pos().y() < 0)
        return; //out of range


    this->curr_pos = QPoint(ev->pos().x()/32,ev->pos().y()/32);
    if (this->click_pos.x() >= 0)
    {
        QImage newimg = QImage(this->current_image);
        QPainter painter;
        painter.begin(&newimg);
        painter.setPen(QPen(Qt::black,3));


        int rect_x = qAbs(this->curr_pos.x()-this->click_pos.x())+1;
        int rect_y = qAbs(this->curr_pos.y()-this->click_pos.y())+1;
        int start_x = qMin(this->curr_pos.x(), this->click_pos.x());
        int start_y = qMin(this->curr_pos.y(), this->click_pos.y());
        painter.drawRect(QRect(start_x*32,start_y*32, 32*rect_x, 32*rect_y));
        painter.end();
        this->setPixmap(QPixmap::fromImage(newimg));
    }
}

void TilesetWidget::mousePressEvent(QMouseEvent *ev)
{
    this->click_pos = QPoint(ev->pos().x()/32, ev->pos().y()/32);

    QImage neu = QImage(this->current_image);
    QPainter painter;
    painter.begin(&neu);
    painter.setPen(QPen(Qt::black,3));


    painter.drawRect(QRect(this->click_pos.x()*32,this->click_pos.y()*32, 32, 32));
    painter.end();
    this->setPixmap(QPixmap::fromImage(neu));

    if (range == 0)
    {
        QList<int> tile_list = this->getCurrentTiles(); //only one click if autotiles
        emit selection_changed(tile_list);
    }
}

void TilesetWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    if (range == 0) return; //no need for autotiles
    this->selection.setTop(qMin(click_pos.y(), curr_pos.y()));
    this->selection.setLeft(qMin(click_pos.x(), curr_pos.x()));
    this->selection.setBottom(qMax(click_pos.y(), curr_pos.y()));
    this->selection.setRight(qMax(click_pos.x(), curr_pos.x()));

    this->click_pos = QPoint(-1,-1);
    QList<int> tile_list = this->getCurrentTiles();
    emit selection_changed(tile_list);
}

QList<int> TilesetWidget::getCurrentTiles()
{
    QList<int> list;

    if (range == 0)
    {
        list.append(1);
        list.append(1);
        list.append(0x30 * click_pos.x());
    }
    else
    {
        list.append(selection.width());
        list.append(selection.height());
        int shift;
        if (range == 1) shift = 0;
        else if (range == 2) shift = 1024;

        for (int y = selection.top(); y <= selection.bottom(); y++)
        {
            for (int x = selection.left(); x <= selection.right(); x++)
            {
                list.append(coordinate_to_bin(QPoint(x,y+shift)));
            }
        }
    }


    return list;
}



int TilesetWidget::coordinate_to_bin(QPoint p)
{
    int val = 0x0180; //topleft value
    val += p.x();
    val += p.y() * 8;
    return val;
}

QPoint TilesetWidget::bin_to_coordinate(int b)
{
    b -= 0x0180; //remove offset
    return QPoint(b % 8, b/8);
}

void TilesetWidget::change_tileset(int id)
{
    this->img = &this->system->tileset_hash.value(id)->tileset;
    this->current_tileset = this->system->tileset_hash.value(id);
    this->updateView();
}

