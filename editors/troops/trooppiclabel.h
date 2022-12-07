#ifndef TROOPPICLABEL_H
#define TROOPPICLABEL_H

#include <QObject>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QDir>

#include <QJsonArray>


class RPGEditorController;

class TroopPicLabel : public QLabel
{
    Q_OBJECT
public:
    TroopPicLabel(QWidget *parent);

    void setEC(RPGEditorController *ec);


    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    //void arrange();
    void remove_current();



public slots:
    void redraw();

private:
    bool left_clicked = false;
    RPGEditorController *ec;

    QJsonArray members;
    QRect bounding_rects[8];

    int marked_member = -1;
    QPoint rel_pos;

};

#endif // TROOPPICLABEL_H
