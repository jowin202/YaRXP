#ifndef RPGANIMATION_H
#define RPGANIMATION_H

#include <QObject>
#include "rpgstring.h"
#include "rpganimationtiming.h"
#include "rpganimationframe.h"

class RPGAnimation : public QObject
{
    Q_OBJECT
public:
    explicit RPGAnimation(QObject *parent = nullptr);

    int id;
    int position;
    int animation_hue;
    int frame_max;

    RPGString animation_name;
    RPGString name;

    QList<RPGAnimationFrame*> frames;
    QList<RPGAnimationTiming*> timings;

    QStringList param_order;
signals:

};

#endif // RPGANIMATION_H
