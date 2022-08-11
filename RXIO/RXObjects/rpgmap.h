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


    int width;
    int height;
    int tileset_id;
    int encounter_step;

    bool autoplay_bgs;
    bool autoplay_bgm;

    RPGEvent *event_on_pos(QPoint pos);
    void fix_autotile_stuff(QPoint start, int layer);

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
