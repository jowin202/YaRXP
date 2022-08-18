#ifndef RPGANIMATIONTIMING_H
#define RPGANIMATIONTIMING_H

#include <QObject>
#include "rpgaudiofile.h"

class RPGAnimationTiming : public QObject
{
    Q_OBJECT
public:
    explicit RPGAnimationTiming(QObject *parent = nullptr);

    int frame;
    int flash_duration;
    int flash_scope;
    int condition;

    RPGAudioFile se;

    int r,g,b, alpha;


    QStringList param_order;
signals:

};

#endif // RPGANIMATIONTIMING_H
