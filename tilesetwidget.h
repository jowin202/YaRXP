#ifndef TILESETWIDGET_H
#define TILESETWIDGET_H

#include <QLabel>
#include <QPixmap>
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QEvent>
#include <QDir>

#include "RXIO/RXObjects/rpgtileset.h"
#include "RXIO/RXObjects/rpgsystem.h"

class TilesetWidget : public QLabel
{
    Q_OBJECT

public:
    TilesetWidget(QWidget *parent);
    void setSystem(RPGSystem *system)
    { this->system = system; }

    void setRange(int range)
    { this->range = range; }

    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    QList<int> getCurrentTiles();
    int coordinate_to_bin(QPoint p);
    QPoint bin_to_coordinate(int b);


signals:
    void selection_changed(QList<int>);

public slots:
    void change_tileset(int id);
    void updateView();

private:
    RPGTileset *current_tileset = 0;
    QImage current_image;
    QPoint curr_pos;
    QPoint click_pos;
    QRect selection;
    QImage *img = 0;

    RPGSystem *system;

    int range;
};

#endif // TILESETWIDGET_H
