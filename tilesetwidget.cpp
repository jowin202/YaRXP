#include "tilesetwidget.h"

TilesetWidget::TilesetWidget(QWidget *parent)
{
    this->setMouseTracking(true);
    this->click_pos = QPoint(-1,-1);

    this->updateView();

}

void TilesetWidget::updateView()
{
    this->setPixmap(QPixmap::fromImage(*img));
}

void TilesetWidget::mouseMoveEvent(QMouseEvent *ev)
{
    if (ev->pos().x()/32 > 7)
        return; //out of range

    this->curr_pos = QPoint(ev->pos().x()/32,ev->pos().y()/32);
    if (this->click_pos.x() >= 0)
    {
        QImage newimg = QImage(*img);
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

    QImage neu = QImage(*img);
    QPainter painter;
    painter.begin(&neu);
    painter.setPen(QPen(Qt::black,3));


    painter.drawRect(QRect(this->click_pos.x()*32,this->click_pos.y()*32, 32, 32));
    painter.end();
    this->setPixmap(QPixmap::fromImage(neu));

}

void TilesetWidget::mouseReleaseEvent(QMouseEvent *ev)
{
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
    QList<int> liste;
    liste.append(selection.width());
    liste.append(selection.height());

    for (int y = selection.top(); y <= selection.bottom(); y++)
    {
        for (int x = selection.left(); x <= selection.right(); x++)
        {
            liste.append(coordinate_to_bin(QPoint(x,y)));
        }
    }

    return liste;
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
    this->img = &this->tilesets->value(id)->tileset;
    this->updateView();
}

