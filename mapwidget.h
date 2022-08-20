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
#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgmap.h"

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



    void setSystem(RPGSystem* system)
    { this->system = system; }


    enum {ZOOM_25, ZOOM_50, ZOOM_100};
    void set_zoom(int zoom)
    {
        this->zoom = zoom;
        if (zoom == ZOOM_25) tile = 8;
        else if (zoom == ZOOM_50) tile = 16;
        else tile = 32;
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

    QRect get_rectangle()
    {
        QPoint topleft( (rectangle_point1.x() < rectangle_point2.x() ? rectangle_point1.x() : rectangle_point2.x()),
                        (rectangle_point1.y() < rectangle_point2.y() ? rectangle_point1.y() : rectangle_point2.y()));
        QPoint bottomright( (rectangle_point1.x() > rectangle_point2.x() ? rectangle_point1.x() : rectangle_point2.x()),
                            (rectangle_point1.y() > rectangle_point2.y() ? rectangle_point1.y() : rectangle_point2.y()));
        return QRect(topleft, bottomright);
    }



    enum {PEN, RECT, FLOOD, SELECT, EVENT};
    void set_mode(int mode);
    void destroy_selection_rectangle();

public slots:
    void set_map(RPGMap *map);
    void set_brush(QList<int> brush);
    void redraw();
    void draw_rectangle();

    void prepare_context_menu( const QPoint & pos );
    void show_event_dialog();


    //for selection
    QList<int> do_copy();
    QList<int> do_cut();
    void do_delete();
    void do_paste(QList<int> data);



private:
    RPGMap *map = 0;
    int mode;
    int height;
    int width;
    int current_layer;
    QImage current_pic;
    int tile = 32;
    int zoom = ZOOM_100;
    QPoint rectangle_point1;
    QPoint rectangle_point2;
    QPoint draw_from_position; // for moving brush
    bool select_rectangle_visible = false;
    bool select_rectangle_move = false;
    QPoint select_rectangle_move_diff;
    QList <int> brush;
    QImage selection_tmp; //backup for merging
    QRect select_rectangle_tmp1; //for source
    QRect select_rectangle_tmp2; //for destination

    QPoint curr_pos;
    bool mouse_pressed_left = false;
    bool mouse_pressed_right = false;
    bool dim_other_layers = false;
    bool show_all_layers = true;


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


    RPGSystem *system;
};

#endif // MAPWIDGET_H
