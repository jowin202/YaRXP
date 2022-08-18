#ifndef RPGMAP_H
#define RPGMAP_H

#include <QObject>
#include "rpgevent.h"
#include "rpgaudiofile.h"
#include "rpgtileset.h"
#include "rpgstring.h"

class RPGMap : public QObject
{
    Q_OBJECT
public:
    explicit RPGMap(QObject *parent = nullptr);

    RPGEvent *event_on_pos(QPoint pos);
    QImage create_map_image(int zoom, bool dim_other, bool show_current_and_below, int current_layer, RPGTileset *tileset);


    enum {ZOOM_25, ZOOM_50, ZOOM_100};


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
