#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <iostream>

#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QEvent>

class MapWidget : public QLabel
{
    Q_OBJECT
public:
    MapWidget(QWidget *parent);
    ~MapWidget();
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    int array_position(QPoint p, int layer);
    void set_layer(int layer);

    int coordinate_to_bin(QPoint p);
    QPoint bin_to_coordinate(int b);

public slots:
    void set_selection(QList<int> vars);
    void redraw();

private:
    QImage img = QImage("/home/johannes/RPG_maker/Pokemon Essentials v19.1 2021-05-22/Graphics/Tilesets/Outside.png");
    QImage img2 = QImage("/home/johannes/RPG_maker/pokemon_decrypted/Graphics/Tilesets/RSEFRLG Complete.png");
    int height;
    int width;
    int current_layer;
    QImage current_pic;
    int selection_rectangle_x = 1;
    int selection_rectangle_y = 1;
    QList <int> selection_vars;
    QPoint curr_pos;
    int *map_values = 0;


};

#endif // MAPWIDGET_H
