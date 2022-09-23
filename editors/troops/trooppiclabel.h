#ifndef TROOPPICLABEL_H
#define TROOPPICLABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QDir>

class RPGTroopMember;
class RPGTroop;
class RPGSystem;
class TroopPicLabel : public QLabel
{
    Q_OBJECT
public:
    TroopPicLabel(QWidget *parent);

    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    void setData(RPGSystem *system, RPGTroop *troop);
    void arrange();
    void clear_troop();
    void remove_current();
    void add_member(int enemy_id);

    int get_member_count();
    QStringList get_enemies_list(); //max 8

    void redraw();

private:
    bool left_clicked = false;
    RPGSystem *system;
    RPGTroop *troop;
    QList<RPGTroopMember*> members;

    int marked_member = -1;
    QPoint rel_pos;

};

#endif // TROOPPICLABEL_H
