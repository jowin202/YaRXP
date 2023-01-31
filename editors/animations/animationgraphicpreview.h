#ifndef ANIMATIONGRAPHICPREVIEW_H
#define ANIMATIONGRAPHICPREVIEW_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include <QDebug>


class RPGDB;

class AnimationGraphicPreview : public QLabel
{
    Q_OBJECT
public:
    AnimationGraphicPreview(QWidget *parent);
    void setDB(RPGDB *db) { this->db = db; }
    void set_image(QString filename, int hue);

    void update();
    void mousePressEvent(QMouseEvent *e);

signals:
    void cell_chosen(int);

private:
    RPGDB *db = 0;
    QString filename;
    int hue = 0;
    int current_cell = -1;
};

#endif // ANIMATIONGRAPHICPREVIEW_H
