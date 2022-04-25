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
    int shitty_mod(int a, int m)
    {
        //thx to MW, bro
        if (a <= 0)
            return (-a)%m;
        return (m-(a%m))%m;
    }

    int coordinate_to_bin(QPoint p);
    QPoint bin_to_coordinate(int b);

    enum {PEN, RECT, FLOOD, SELECT};
    void set_mode(int mode);

public slots:
    void set_brush(QList<int> vars);
    void redraw();
    void draw_brush_rectangle();

    //for selection
    void draw_selection_rectangle();
    QList<int> do_copy();
    QList<int> do_cut();
    void do_delete();
    void do_paste(QList<int> data);
    void merge_selection();

    void flood_fill(QPoint clicked, QPoint next);

private:
    QImage img = QImage("/home/johannes/RPG_maker/Pokemon Essentials v19.1 2021-05-22/Graphics/Tilesets/Outside.png");
    QImage img2 = QImage("/home/johannes/RPG_maker/pokemon_decrypted/Graphics/Tilesets/RSEFRLG Complete.png");
    int mode;
    int height;
    int width;
    int current_layer;
    QImage current_pic;
    QList <int> brush_vars;
    QPoint curr_pos;
    int *map_values = 0;
    bool mouse_pressed_left = false;
    bool mouse_pressed_right = false;
    bool dim_other_layers = false;

    //QPoint tmp_point_brush_rectangle; //rightclick
    QPoint left_click_pos; //when drawing multiple tiles
    QPoint brush_first_click_pos; //right click on map
    QRect brush_rectangle;
    QPoint brush_rectangle_mouse_alignment;


    //for selection
    bool selection_button = false;
    bool moving_selection = false;
    bool selection_rectangle_is_released = false;
    QPoint selection_first_click_pos;
    QPoint move_pos_temp;
    QRect selection_rectangle;
    QList<int> selection_list;

};

#endif // MAPWIDGET_H
