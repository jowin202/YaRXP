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
    void set_dim(bool dim);

    int coordinate_to_bin(QPoint p);
    QPoint bin_to_coordinate(int b);

public slots:
    void set_selection(QList<int> vars);
    void redraw();
    void draw_selection_rectangle();

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
    bool mouse_pressed_left = false;
    bool mouse_pressed_right = false;
    bool dim_other_layers = false;

    //QPoint tmp_point_selection_rectangle; //rightclick
    QRect tmp_selection;
    QPoint left_click_pos; //when drawing multiple tiles

};

#endif // MAPWIDGET_H
