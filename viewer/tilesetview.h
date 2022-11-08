#ifndef TILESETVIEW_H
#define TILESETVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>


class RPGSystem;
class RPGTileset;
class TilesetRectangle;

class TilesetView : public QGraphicsView
{
    Q_OBJECT
public:
    TilesetView(QWidget *parent);



    RPGSystem *system;
    void setSystem(RPGSystem *system) {this->system = system;}

    RPGTileset *current_tileset;
    void set_tileset(int id);


    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


    bool clicked = false;
    TilesetRectangle *rectangle = 0;
    QPoint last_pos;
    int max_height = 0;


public slots:
    void select_tile(int value);


signals:
    void selection_changed(QList<int>);

};

#endif // TILESETVIEW_H
