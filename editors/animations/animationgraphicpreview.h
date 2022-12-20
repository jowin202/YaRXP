#ifndef ANIMATIONGRAPHICPREVIEW_H
#define ANIMATIONGRAPHICPREVIEW_H

#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QPainter>

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

private:
    RPGDB *db = 0;
    QString filename;
    int hue = 0;
};

#endif // ANIMATIONGRAPHICPREVIEW_H
