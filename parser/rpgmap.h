#ifndef RPGMAP_H
#define RPGMAP_H

#include <QObject>
#include "rpgevent.h"
#include "rpgaudiofile.h"
#include "rpgtileset.h"

class RPGMap : public QObject
{
    Q_OBJECT
public:
    explicit RPGMap(QObject *parent = nullptr);
    void setParameter(QString param, int val);
    void setParameter(QString param, bool val);
    void setParameter(QString param, RPGAudioFile *val);

    void debug();

    int width;
    int height;
    int tileset_id;
    int encounter_step;

    bool autoplay_bgs;
    bool autoplay_bgm;

    //encounter_list (empty for pokemon)
    //z=3
    //size=x*y*z don't need that
    QList<int> data;

    QList<RPGEvent*> events;

    RPGAudioFile *bgs = 0, *bgm = 0;

    RPGTileset *tileset;


signals:

private:


};

#endif // RPGMAP_H
