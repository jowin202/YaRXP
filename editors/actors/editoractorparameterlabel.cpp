#include "editoractorparameterlabel.h"

EditorActorParameterLabel::EditorActorParameterLabel(QWidget *parent) : QLabel(parent)
{
    this->left_click = false;
}

void EditorActorParameterLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        this->left_click = true;
        int x = ev->x()/4;
        int y = qRound((250 - ev->y())/250.0 * this->maxval);
        if (y==0) y=1;
        this->values[x] = y;
        emit curve_changed(x,y);
    }
}

void EditorActorParameterLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if (this->left_click && ev->pos().x() >= 0 && ev->pos().x() < 400 && ev->pos().y() >= 0 && ev->y() <= 250)
    {
        int x = ev->x()/4;
        int y = qRound((250 - ev->y())/250.0 * this->maxval);
        if (y==0) y=1;
        this->values[x] = y;
        emit curve_changed(x,y);
    }
}

void EditorActorParameterLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
        this->left_click = false;
}

