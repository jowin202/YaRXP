#ifndef ANIMATIONLABEL_H
#define ANIMATIONLABEL_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPainter>

class AnimationLabel : public QLabel
{
    Q_OBJECT
public:
    AnimationLabel(QWidget *parent);
};

#endif // ANIMATIONLABEL_H
