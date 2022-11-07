#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>



class MapRectangle;
class MapTile;
class RPGMap;
class RPGSystem;
class RPGTileset;

enum modes { PEN, SELECT, EVENT };

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



    RPGSystem *system;
    void setSystem(RPGSystem *system) {this->system = system;}

    void set_map(int id);
    void set_layer(int layer);
    void set_current_and_below(bool val);



    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;




    tile_options opt;

signals:
    void zoom_in();
    void zoom_out();

public slots:
    void redraw();
    void set_brush(QList<int> data);

private:
    bool changes_made = false;
    RPGMap *map;
    //int current_layer = 0;
    RPGTileset *tileset;


    QList<int> brush;

    MapRectangle *rectangle = 0;
    QPoint last_pos;
    QPoint draw_from_pos;
    QPoint rightclick_from_pos;
    QPoint rectangle_offset = QPoint(0,0);
    bool left_button_clicked = false;
    bool right_button_clicked = false;
};

#endif // MAPVIEW_H
