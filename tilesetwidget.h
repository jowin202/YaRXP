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

#include "parser/rpgtileset.h"

class TilesetWidget : public QLabel
{
    Q_OBJECT

public:
    TilesetWidget(QWidget *parent);
    void setTilesets(QHash<int, RPGTileset*> *tilesets)
    { this->tilesets = tilesets; }

    void updateView();
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    QList<int> getCurrentTiles();
    int coordinate_to_bin(QPoint p);
    QPoint bin_to_coordinate(int b);

signals:
    //void selection_changed();
    void selection_changed(QList<int>);

public slots:
    void change_tileset(int id);

private:
    QHash<int, RPGTileset*> *tilesets;
    RPGTileset *tileset;
    QPoint curr_pos;
    QPoint click_pos;
    QRect selection;
    QImage *img = 0;
};

#endif // TILESETWIDGET_H
