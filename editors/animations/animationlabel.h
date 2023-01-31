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

private:
    int current_frame = -1;
    RPGEditorController * ec = 0;

    QImage animation_graphic;
    QString current_animation_graphic;
    int current_hue = 0;

    QMap<int,QRect> current_red_rectangles;
    int current_rectangle = -1;

};

#endif // ANIMATIONLABEL_H
