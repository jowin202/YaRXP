#ifndef RPGMAP_H
#define RPGMAP_H

#include <QObject>
#include "rpgevent.h"
#include "rpgaudiofile.h"

class RPGMap : public QObject
{
    Q_OBJECT
public:
    explicit RPGMap(QObject *parent = nullptr);

signals:

private:
    int width;
    int height;
    bool autoplay_bgs;
    bool autoplay_bgm;
    //encounter_list (empty for pokemon)
    //z=3
    //size=x*y*z don't need that
    QList<int> data;

    QList<RPGEvent*> events;

    RPGAudioFile bgs, bgm;
    int tileset_id;
    int encounter_step;


};

#endif // RPGMAP_H
