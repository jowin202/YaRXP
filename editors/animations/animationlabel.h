#ifndef ANIMATIONLABEL_H
#define ANIMATIONLABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPainter>


class RPGEditorController;

class AnimationLabel : public QLabel
{
    Q_OBJECT
public:
    AnimationLabel(QWidget *parent);
    void setEC(RPGEditorController *ec) {this->ec = ec;}

    void update(int frame);

private:
    int current_frame = -1;
    RPGEditorController * ec = 0;
};

#endif // ANIMATIONLABEL_H
