#include "editoractorparameterlabel.h"

EditorActorParameterLabel::EditorActorParameterLabel(QWidget *parent) : QLabel(parent)
{
    this->left_click = false;
}

void EditorActorParameterLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        this->last_x = -1;
        this->left_click = true;
        int x = ev->position().x()/4;
        if (x==0) return;
        int y = qRound((250 - ev->position().y())/250.0 * this->maxval);
        if (y==0) y=1;
        this->values[x] = y;
        emit curve_changed(x,y);
    }
}

void EditorActorParameterLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if (this->left_click && ev->position().x() >= 0 && ev->position().x() < 400 && ev->position().y() >= 0 && ev->position().y() <= 250)
    {
        int x = ev->position().x()/4;
        if (x==0) return;
        int y = qRound((250 - ev->position().y())/250.0 * this->maxval);
        if (y==0) y=1;
        this->values[x] = y;
        emit curve_changed(x,y);

        if (qAbs(x-last_x) > 1 && last_x > 0)
        {
            qreal k = (this->values[x] - this->values[last_x]*1.0)/(x-last_x);
            for (int i = qMin(x,last_x)+1; i < qMax(x,last_x); i++)
            {
                this->values[i] = this->values[qMin(x,last_x)] + k*(i-qMin(x,last_x));
            }
        }

        this->last_x = x;
    }
}

void EditorActorParameterLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)
        this->left_click = false;
}

