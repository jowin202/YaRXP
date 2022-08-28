#ifndef RPGMAP_H
#define RPGMAP_H

#include <QObject>
#include <QFile>
#include "rpgevent.h"
#include "rpgaudiofile.h"
#include "rpgtileset.h"
#include "rpgstring.h"

class RPGSystem;

class RPGMap : public QObject
{
    Q_OBJECT
public:
    explicit RPGMap(QObject *parent = nullptr);
    enum {ZOOM_25, ZOOM_50, ZOOM_100};

    ~RPGMap();

    int array_position(QPoint p, int layer)
    {return p.x() + p.y() * this->width + this->height * this->width * layer;}

    QPoint pos_from_array_index(int index)
    { int x = index%this->width; int y = ((index-x)/this->width)%this->height; return QPoint(x,y); }

    int layer_from_array_index(int index)
    { return index / (this->height * this->width); }


    RPGEvent *event_on_pos(QPoint pos);
    RPGEvent *event_by_id(int id);
    QImage create_map_image(int zoom, bool dim_other, bool show_current_and_below, int current_layer, RPGTileset *tileset);
    QList<int> get_elements_in_rectangle(QRect rect, int fromlayer, int tolayer);
    void delete_elements_in_rectangle(QRect rect, int fromlayer, int tolayer);
    void put_elements_from_list(QPoint pos, QPoint rel_pos, QList<int> list, int fromlayer, int tolayer);
    void move_map_part(QRect rect, QPoint pos);


    void shift_map_x(int tiles);
    void shift_map_y(int tiles);
    void extend_or_crop_x(int tiles);
    void extend_or_crop_y(int tiles);



    void load_event_graphics(RPGSystem *system);


    //params
    int width;
    int height;
    int tileset_id;
    int encounter_step;

    bool autoplay_bgs;
    bool autoplay_bgm;

    QList<int> data;
    QList<RPGEvent*> events;
    RPGAudioFile bgs;
    RPGAudioFile bgm;

    QList<int> encounter_list;

    QStringList param_order;
signals:

private:


};

#endif // RPGMAP_H
