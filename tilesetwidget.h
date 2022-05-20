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
#include "settings.h"

class TilesetWidget : public QLabel
{
    Q_OBJECT

public:
    TilesetWidget(QWidget *parent);
    void setSettings(Settings *settings)
    { this->settings = settings; }

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
    RPGTileset *current_tileset = 0;
    QImage current_image;
    QPoint curr_pos;
    QPoint click_pos;
    QRect selection;
    QImage *img = 0;

    Settings *settings;
};

#endif // TILESETWIDGET_H
