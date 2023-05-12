#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>
#include <QRect>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>

#include <QAction>
#include <QMenu>

#include "mapselectrectangle.h"
#include "maptile.h"
#include "undodb.h"

#include "RXIO2/rpgmapcontroller.h"

class MapSelectRectangle;
class MapRectangle;
class MapTile;


struct tile_options {
    int mode;
    int layer;
    bool current_and_below;
    //bool dim;
    QPoint marked_tile; //events
};

class MapView : public QGraphicsView
{
    Q_OBJECT
public:
    MapView(QWidget *parent);

    enum modes { PEN, SELECT, EVENT };

    void setDB(RPGDB *db) { this->db = db; this->mc.setDB(db); }

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
        if (dir == LEFT || dir == RIGHT) this->mc.shift_map(dir==LEFT ? -n : n,0);
        else if (dir == UP || dir == DOWN) this->mc.shift_map(0,dir==UP ? -n : n);
        this->redraw();
    }
    void extend(int dir,int n)
    {
        this->changes_made = true;
        if (dir == RIGHT) this->mc.set_size(this->mc.get_width()+n,this->mc.get_height());
        else if (dir == DOWN) this->mc.set_size(this->mc.get_width(),this->mc.get_height()+n);
        else if (dir == LEFT) {this->mc.set_size(this->mc.get_width()+n,this->mc.get_height()); this->mc.shift_map(n,0);}
        else if (dir == UP) {this->mc.set_size(this->mc.get_width(),this->mc.get_height()+n); this->mc.shift_map(0,n);}
        this->redraw();
    }
    void crop(int dir, int n)
    {
        this->changes_made = true;
        if (dir == RIGHT) this->mc.set_size(this->mc.get_width()-n,this->mc.get_height());
        else if (dir == DOWN) this->mc.set_size(this->mc.get_width(),this->mc.get_height()-n);
        else if (dir == LEFT) {this->mc.shift_map(-n,0); this->mc.set_size(this->mc.get_width()-n,this->mc.get_height()); }
        else if (dir == UP) {this->mc.shift_map(0,-n); this->mc.set_size(this->mc.get_width(),this->mc.get_height()-n);}
        this->redraw();
    }




    RPGMapController *get_mc() { return &this->mc; } //for editor dialog event handling in troops and common events
signals:
    void zoom_in();
    void zoom_out();
    void one_tile_selected(int);
    void mouse_over_coordinates(int,int);
    void jump_to_map(int);

public slots:
    void redraw();
    void set_brush(QList<int> data);
    void do_copy()
    {
        if (opt.mode == SELECT && this->select_rectangle != 0)
        {
            QJsonArray array;
            for (int i = 0; i < this->select_rectangle->brush.length(); i++)
                array.append(this->select_rectangle->brush.at(i));
            QJsonDocument doc(array);
            QSettings settings;
            settings.setValue("copied_map_part", doc.toJson(QJsonDocument::Compact));
        }
        else if (opt.mode == EVENT)
        {
            QJsonObject ev = this->mc.event_on_pos(opt.marked_tile);
            if (ev.contains("RXClass"))
            {
                QJsonDocument doc(ev);
                QSettings settings;
                settings.setValue("copied_event", doc.toJson(QJsonDocument::Compact));
            }
        }
    }
    void do_cut()
    {
        this->do_copy();
        this->do_delete();
    }
    void do_delete()
    {
        if (opt.mode == EVENT)
        {
            this->undo.push(mc.get_current_map_id(),mc.current_map()->object());
            QJsonObject ev = this->mc.event_on_pos(opt.marked_tile);
            if (ev.contains("RXClass"))
                mc.remove_event_by_id(ev.value("@id").toInt());

            MapTile *tile;
            if ( (tile = ((MapTile*)this->scene()->itemAt(32*opt.marked_tile,QTransform()))) != 0)
                tile->update();
        }
        else if (opt.mode == SELECT && this->select_rectangle != 0)
        {
            this->kill_select_rectangle();
        }
    }
    void do_paste()
    {
        if (opt.mode == SELECT || opt.mode == PEN)
        {
            if (this->select_rectangle != 0)
            {
                //save current selection
                this->select_rectangle->save_to_map();
                this->kill_select_rectangle();
            }
            QSettings settings;
            QJsonParseError err;
            QByteArray json = settings.value("copied_map_part").toByteArray();
            QJsonDocument doc = QJsonDocument::fromJson(json, &err);
            if (err.error != QJsonParseError::NoError) return;

            QList<int> data;
            for (int i = 0; i < doc.array().count(); i++)
                data.append(doc.array().at(i).toInt());

            this->set_mode(SELECT);
            this->select_rectangle = new MapSelectRectangle(&this->mc,0,0);
            this->select_rectangle->setSize(data.at(0),data.at(1));
            this->select_rectangle->set_brush(data, &opt);
            this->select_rectangle->setPos(32*this->last_pos);
            this->scene()->addItem(this->select_rectangle);
        }
        else if (opt.mode == EVENT)
        {
            QSettings settings;
            QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_event").toByteArray());
            if (!mc.event_on_pos(opt.marked_tile).contains("RXClass"))
            {
                int id = mc.get_next_event_id();
                QJsonObject ev = doc.object();
                ev.insert("@x", opt.marked_tile.x());
                ev.insert("@y", opt.marked_tile.y());
                ev.insert("@id", id);
                this->undo.push(mc.get_current_map_id(),mc.current_map()->object());
                mc.set_event_by_id(id, ev);
                MapTile *tile;
                if ( (tile = ((MapTile*)this->scene()->itemAt(32*opt.marked_tile,QTransform()))) != 0)
                    tile->update();
            }
        }
    }
    void do_undo()
    {
        QJsonObject obj = undo.pop(mc.get_current_map_id());
        if (obj != QJsonObject())
            mc.current_map()->setObject(obj);
        this->scene()->update();
    }
    void prepare_context_menu(QPoint pos)
    {
        if (opt.mode == EVENT)
        {
            QMenu menu;
            if (mc.event_on_pos(opt.marked_tile).contains("RXClass"))
                menu.addAction(&this->action_editevent);
            else
                menu.addAction(&this->action_newevent);
            menu.addSeparator();
            menu.addAction(&this->action_cut);
            menu.addAction(&this->action_copy);
            menu.addAction(&this->action_paste);
            menu.addAction(&this->action_delete);
            menu.addSeparator();
            menu.addAction(&this->action_set_start);
            menu.addAction(&this->action_follow_teleport);
            menu.exec(this->mapToGlobal(pos));
        }
    }
    void edit_event_on_pos(QPoint pos);



private:
    bool changes_made = false;
    RPGDB *db;
    RPGMapController mc;
    //int current_layer = 0;


    //draw rectangle
    MapRectangle *rectangle = 0;
    QList<int> brush;
    QPoint last_pos;
    QPoint draw_from_pos;
    QPoint rightclick_from_pos;
    QPoint rectangle_offset = QPoint(0,0);
    QPoint last_valid_pos_in_draw_rectangle; //needed for calculating rectangle offset
    bool left_button_clicked = false;
    bool right_button_clicked = false;


    //event mode
    QPoint event_moving_from_pos = QPoint(-1,-1);
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
        this->scene()->update(this->select_rectangle->pos().x()-32,this->select_rectangle->pos().y()-32,32*this->select_rectangle->size().width()+64,32*this->select_rectangle->size().height()+64);
        //this->select_rectangle->update();
        delete this->select_rectangle;
        this->select_rectangle = 0;
    }

    QAction action_newevent;
    QAction action_editevent;
    QAction action_cut;
    QAction action_copy;
    QAction action_paste;
    QAction action_delete;
    QAction action_set_start;
    QAction action_follow_teleport;


    UndoDB undo;
};

#endif // MAPVIEW_H
