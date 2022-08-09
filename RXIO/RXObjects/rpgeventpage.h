#ifndef RPGEVENTPAGE_H
#define RPGEVENTPAGE_H

#include <QImage>
#include <QObject>
#include "rpgeventcommand.h"
#include "rpgmoveroute.h"

class RPGEventPage : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventPage(QObject *parent = nullptr);
    int move_speed;
    int move_frequency;
    int move_type;
    int trigger;

    bool walk_anime;
    bool step_anime;
    bool through;
    bool direction_fix;
    bool always_on_top;

    RPGMoveRoute move_route;


    QList<RPGEventCommand*> list;




    //RPGEventPageCondition *condition;
    //parse condition here
    int switch1_id;
    int switch2_id;
    int variable_id;

    bool self_switch_valid;
    bool variable_valid;
    bool switch1_valid;
    bool switch2_valid;

    int variable_value;
    int self_switch_ch; //A=1, B=2, C=3, D=4


    //RPGEventPageGraphic *graphic;
    //parse graphic here
    int direction;
    int blend_type;
    int tile_id;
    int pattern;
    int character_hue;
    int opacity;
    RPGString character_name;

    QImage graphics;


signals:

private:


};

#endif // RPGEVENTPAGE_H
