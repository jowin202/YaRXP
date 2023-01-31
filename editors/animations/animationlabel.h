#ifndef ANIMATIONLABEL_H
#define ANIMATIONLABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMap>

class RPGEditorController;

class AnimationLabel : public QLabel
{
    Q_OBJECT
public:
    AnimationLabel(QWidget *parent);
    void setEC(RPGEditorController *ec) {this->ec = ec;}

    void update(int frame);
    void set_animation_graphic(QString name, int hue);

    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    void mouseDoubleClickEvent(QMouseEvent *e);

public slots:
    void set_pattern(int p) { this->pattern = p;}

private:
    int current_frame = -1;
    RPGEditorController * ec = 0;

    QImage animation_graphic;
    QString current_animation_graphic;
    int current_hue = 0;

    QMap<int,QRect> current_red_rectangles;
    int current_rectangle = -1;
    bool is_moving = false;

    int x=0;//internal use only
    int y=0;

    int pattern = 0;

};

#endif // ANIMATIONLABEL_H
