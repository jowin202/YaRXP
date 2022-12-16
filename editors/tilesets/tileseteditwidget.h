#ifndef TILESETEDITWIDGET_H
#define TILESETEDITWIDGET_H

#include <QObject>
#include <QtMath>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QScrollBar>

#include "tile.h"

class RPGDB;
class RPGEditorController;

class TilesetEditWidget : public QGraphicsView
{
    Q_OBJECT
public:
    TilesetEditWidget(QWidget *parent);

    void setEC(RPGEditorController *ec) {this->ec = ec; }
    void set_mode(int mode);

    void change_priority(int pos, int val);
    void change_passage(int pos, int val);
    void change_terrain(int pos, int val);



public slots:
    void update_tileset();



private:
    RPGEditorController *ec;
    int tiles_num;
    QGraphicsPixmapItem *autotiles_items[8];

    tile_options opt;
};

#endif // TILESETEDITWIDGET_H
