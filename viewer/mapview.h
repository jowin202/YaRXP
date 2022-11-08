#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

#include "RXIO/RXObjects/rpgmap.h"
#include "mapselectrectangle.h"


class MapSelectRectangle;
class MapRectangle;
class MapTile;
class RPGMap;
class RPGSystem;
class RPGTileset;


struct tile_options {
    int mode;
    int layer;
    bool current_and_below;
    //bool dim;
};

class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    MapView(QWidget *parent);

    enum modes { PEN, SELECT, EVENT };


    RPGSystem *system;
    void setSystem(RPGSystem *system) {this->system = system;}

    void set_mode(int mode);
    void set_map(int id);
    void set_layer(int layer);
    void set_current_and_below(bool val);



    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    tile_options opt;


    //map tools
    enum {LEFT, RIGHT, UP, DOWN};
    void shift(int dir, int n)
    {
        this->changes_made = true;
        if (dir == LEFT || dir == RIGHT) this->map->shift_map_x(dir==LEFT ? -n : n);
        else if (dir == UP || dir == DOWN) this->map->shift_map_y(dir==UP ? -n : n);
        this->redraw();
    }
    void extend(int dir,int n)
    {
        this->changes_made = true;
        if (dir == RIGHT) this->map->extend_or_crop_x(n);
        else if (dir == DOWN) this->map->extend_or_crop_y(n);
        else if (dir == LEFT) {this->map->extend_or_crop_x(n); this->map->shift_map_x(n);}
        else if (dir == UP) {this->map->extend_or_crop_y(n);this-> map->shift_map_y(n);}
        this->redraw();
    }
    void crop(int dir, int n)
    {
        this->changes_made = true;
        if (dir == RIGHT) this->map->extend_or_crop_x(-n);
        else if (dir == DOWN) this->map->extend_or_crop_y(-n);
        else if (dir == LEFT) {this->map->shift_map_x(-n); this->map->extend_or_crop_x(-n);}
        else if (dir == UP) {this->map->shift_map_y(-n); this->map->extend_or_crop_y(-n);}
        this->redraw();
    }

    QList<int> do_copy()
    { return this->select_rectangle->brush; }
    QList<int> do_cut()
    {
        QList<int> brush = this->select_rectangle->brush;
        this->kill_select_rectangle();
        return brush;
    }
    void do_delete()
    {
        if (opt.mode == EVENT)
        {
            //TODO
        }
        else if (opt.mode == SELECT && this->select_rectangle != 0)
        {
            this->kill_select_rectangle();
        }
    }
    void do_paste(QList<int> data)
    {
        if (opt.mode == SELECT && this->select_rectangle != 0)
        {
            //save current selection
            this->select_rectangle->save_to_map(this->map);
            this->kill_select_rectangle();
        }
        this->set_mode(SELECT);
        this->select_rectangle = new MapSelectRectangle(0,0);
        this->select_rectangle->setSize(data.at(0),data.at(1));
        this->select_rectangle->set_brush(data, tileset, &opt);
        this->select_rectangle->setPos(32*this->last_pos);
        this->scene()->addItem(this->select_rectangle);
    }


signals:
    void zoom_in();
    void zoom_out();
    void one_tile_selected(int);
    void hover_coordinates(int,int);

public slots:
    void redraw();
    void set_brush(QList<int> data);

private:
    bool changes_made = false;
    RPGMap *map;
    //int current_layer = 0;
    RPGTileset *tileset;



    //draw rectangle
    MapRectangle *rectangle = 0;
    QList<int> brush;
    QPoint last_pos;
    QPoint draw_from_pos;
    QPoint rightclick_from_pos;
    QPoint rectangle_offset = QPoint(0,0);
    bool left_button_clicked = false;
    bool right_button_clicked = false;


    //event mode
    RPGEvent *event_for_moving = 0;
    bool event_left_button_for_moving = false;


    //select rectangle
    MapSelectRectangle *select_rectangle = 0;
    QPoint select_click = QPoint(0,0);
    QPoint select_rectangle_offset = QPoint(0,0);
    bool select_mouse_button_pressed = false;
    bool is_moving = false;

    void kill_select_rectangle()
    {
        this->is_moving = false;
        this->scene()->removeItem(this->select_rectangle);
        this->select_rectangle->update();
        delete this->select_rectangle;
        this->select_rectangle = 0;
    }


};

#endif // MAPVIEW_H
