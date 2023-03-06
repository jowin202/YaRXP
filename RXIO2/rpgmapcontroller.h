#ifndef RPGMAPCONTROLLER_H
#define RPGMAPCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QPoint>
#include <QPointF>
#include <QImage>
#include <QJsonDocument>

class RPGDB;

class RPGMapController : public QObject
{
    Q_OBJECT
public:
    explicit RPGMapController(QObject *parent = nullptr);

    void setDB(RPGDB *db) {this->db = db; }
    RPGDB *getDB() { return this->db; }
    void setMap(int id, bool load_images = true);

    bool current_map_is_valid() { return (this->doc != 0); }


    int array_position(QPoint p, int layer)
    {return p.x() + p.y() * this->get_width() + this->get_height() * this->get_width() * layer;}

    //QPoint pos_from_array_index(int index)
    //{ int x = index%this->width; int y = ((index-x)/this->width)%this->height; return QPoint(x,y); }

    //int layer_from_array_index(int index)
    //{ return index / (this->height * this->width); }


    int get_autotile_nn(QPoint pos, int layer);



    QImage get_tile_from_pos(QPoint pos, int layer);
    QImage get_tile_from_id(int id);


    QJsonObject event_on_pos(QPoint pos);

    QJsonObject event_by_id(int id);
    void set_event_by_id(int id, QJsonObject event);
    void remove_event_by_id(int id);
    int get_next_event_id();

    bool move_event(QPoint from, QPoint to);

    QList<int> get_elements_in_rectangle(QRect rect, int fromlayer, int tolayer);
    void delete_elements_in_rectangle(QRect rect, int fromlayer, int tolayer);
    void put_elements_from_list(QPoint pos, QPoint rel_pos, QList<int> list, int fromlayer, int tolayer);
    void move_map_part(QRect rect, QPoint pos);


    void shift_map(int xtiles, int ytiles);
    void set_size(int xtiles, int ytiles);


    int get_height();
    int get_width();

    QJsonValue get_jsonvalue(QString key);
    void set_jsonvalue(QString key, QJsonValue value);

    QJsonDocument *current_map() {return doc;}
    int get_current_map_id() { return this->current_map_id; }

private:
    RPGDB *db;
    QJsonDocument *doc = 0;
    int current_map_id = 0;

    QImage tileset_image;
    QImage autotiles[7];

};

#endif // RPGMAPCONTROLLER_H
