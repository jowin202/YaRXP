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
#include <QDir>

#include <QAction>
#include <QMenu>

#include "event_dialogs/eventdialog.h"
#include "settings.h"
#include "parser/rpgmap.h"

class MapWidget : public QLabel
{
    Q_OBJECT
public:
    MapWidget(QWidget *parent);
    ~MapWidget();

    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseDoubleClickEvent(QMouseEvent *ev); //event dialog


    int array_position(QPoint p, int layer)
    {
        return p.x() + p.y() * this->width + this->height * this->width * layer;
    }

    void setSettings(Settings* settings)
    { this->settings = settings; }

    void set_event_mode()
    {
        if (this->selection_rectangle.height() > 0 || this->selection_rectangle.width() > 0)
            this->merge_selection(); //selection should stay if mode changed

        this->mode = EVENT;
        this->current_layer = 2; //show all layers
        this->redraw();
    }
    void set_layer(int layer)
    {
        if (this->mode == EVENT) this->mode = PEN; //leave event_mode when choosing another layer
        this->current_layer = layer;
        this->redraw();
    }
    void set_dim(bool dim)
    {    this->dim_other_layers = dim;
         this->redraw();
    }
    void set_show_other_layers(bool var)
    {
        this->show_all_layers = var;
        this->redraw();
    }
    int shitty_mod(int a, int m)
    {
        //thx to MW, bro
        if (a <= 0)
            return (-a)%m;
        return (m-(a%m))%m;
    }


    int coordinate_to_bin(QPoint p);
    QPoint bin_to_coordinate(int b);

    enum {PEN, RECT, FLOOD, SELECT, EVENT};
    void set_mode(int mode);

public slots:
    void set_map(RPGMap *map);
    void set_brush(QList<int> vars);
    void redraw();
    void draw_events();
    void draw_brush_rectangle();

    void prepare_context_menu( const QPoint & pos );
    void show_event_dialog();


    //for selection
    void draw_selection_rectangle();
    QList<int> do_copy();
    QList<int> do_cut();
    void do_delete();
    void do_paste(QList<int> data);
    void merge_selection();

    void flood_fill(QPoint clicked, QPoint next);

private:
    RPGMap *map = 0;
    QImage *img = new QImage(256,256,QImage::Format_ARGB32);
    int mode;
    int height;
    int width;
    int current_layer;
    QImage current_pic;
    QList <int> brush_vars;
    QPoint curr_pos;
    bool mouse_pressed_left = false;
    bool mouse_pressed_right = false;
    bool dim_other_layers = false;
    bool show_all_layers = true;

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



    //event_move stuff
    QPoint event_move_from_pos;


    //event menu
    RPGEvent *event_for_editing = 0;
    QMenu event_menu_with_new;
    QMenu event_menu_with_edit;

    QAction action_cut, action_copy, action_paste, action_delete;
    QAction action_new;
    QAction action_edit;
    QAction action_player_starting_pos;


    Settings *settings;
};

#endif // MAPWIDGET_H
