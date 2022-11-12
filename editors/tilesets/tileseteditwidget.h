#ifndef TILESETEDITWIDGET_H
#define TILESETEDITWIDGET_H

#include <QObject>
#include <QtMath>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QScrollBar>

#include "tile.h"

class RPGTileset;
class RPGSystem;

class TilesetEditWidget : public QGraphicsView
{
    Q_OBJECT
public:
    TilesetEditWidget(QWidget *parent);

    void setSystem(RPGSystem *system) {this->system = system; }
    void set_data(QList<int> *passages, QList<int> *priorities, QList<int> *terrain);
    void set_mode(int mode);

public slots:
    void set_tileset(QString tileset_image);
    void set_autotile(QString autotile_image, int pos);
    void set_autotile_1(QString autotile_image) { this->set_autotile(autotile_image, 1); };
    void set_autotile_2(QString autotile_image) { this->set_autotile(autotile_image, 2); };
    void set_autotile_3(QString autotile_image) { this->set_autotile(autotile_image, 3); };
    void set_autotile_4(QString autotile_image) { this->set_autotile(autotile_image, 4); };
    void set_autotile_5(QString autotile_image) { this->set_autotile(autotile_image, 5); };
    void set_autotile_6(QString autotile_image) { this->set_autotile(autotile_image, 6); };
    void set_autotile_7(QString autotile_image) { this->set_autotile(autotile_image, 7); };

private:
    RPGTileset *current_tileset;
    RPGSystem *system;
    int tiles_num;
    QGraphicsPixmapItem *autotiles_items[8];

    QList<int> passages;
    QList<int> priorities;
    QList<int> terrain;
    tile_options opt;
};

#endif // TILESETEDITWIDGET_H
