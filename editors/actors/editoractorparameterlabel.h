#ifndef EDITORACTORPARAMETERLABEL_H
#define EDITORACTORPARAMETERLABEL_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>

class EditorActorParameterLabel : public QLabel
{
    Q_OBJECT
public:
    EditorActorParameterLabel(QWidget *parent);

    void set_values(int *values, qreal maxval)
    { this->values = values; this->maxval = maxval; }

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);


signals:
    void curve_changed(int,int);

private:
    bool left_click;
    int *values;
    qreal maxval;
};

#endif // EDITORACTORPARAMETERLABEL_H
