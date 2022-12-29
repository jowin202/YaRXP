#ifndef RPGMAPCONTROLLER_H
#define RPGMAPCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QPoint>
#include <QPointF>
#include <QJsonDocument>

class RPGDB;

class RPGMapController : public QObject
{
    Q_OBJECT
public:
    explicit RPGMapController(QObject *parent = nullptr);

    void setDB(RPGDB *db) {this->db = db; }
    void setMap(int id);



    //int array_position(QPoint p, int layer)
    //{return p.x() + p.y() * this->width + this->height * this->width * layer;}

    //QPoint pos_from_array_index(int index)
    //{ int x = index%this->width; int y = ((index-x)/this->width)%this->height; return QPoint(x,y); }

    //int layer_from_array_index(int index)
    //{ return index / (this->height * this->width); }


    int get_tile_id_from_pos(QPoint pos, int layer);


    QJsonObject event_on_pos(QPoint pos);

    QJsonObject event_by_id(int id);
    void set_event_by_id(int id, QJsonObject event);

    //QImage create_map_image(int zoom, bool dim_other, bool show_current_and_below, int current_layer, RPGTileset *tileset);
    QJsonArray get_elements_in_rectangle(QRect rect, int fromlayer, int tolayer);
    void delete_elements_in_rectangle(QRect rect, int fromlayer, int tolayer);
    void put_elements_from_list(QPoint pos, QPoint rel_pos, QJsonArray list, int fromlayer, int tolayer);
    void move_map_part(QRect rect, QPoint pos);


    void shift_map_x(int tiles);
    void shift_map_y(int tiles);


    int get_height();
    int get_width();
    void set_height(int tiles);
    void set_width(int tiles);




    QJsonValue get_jsonvalue(QString key);
    void set_jsonvalue(QString key, QJsonValue value);

private:
    RPGDB *db;
    QJsonDocument *doc;

};

#endif // RPGMAPCONTROLLER_H
