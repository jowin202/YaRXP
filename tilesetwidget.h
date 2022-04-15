#ifndef TILESETWIDGET_H
#define TILESETWIDGET_H

#include <QLabel>
#include <QPixmap>
#include <QEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QEvent>

class TilesetWidget : public QLabel
{
    Q_OBJECT

public:
    TilesetWidget(QWidget *parent);
    void mouseMoveEvent(QMouseEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    QList<int> getCurrentTiles();
    int coordinate_to_bin(QPoint p);
    QPoint bin_to_coordinate(int b);

signals:
    //void selection_changed();
    void selection_changed(QList<int>);

public slots:
    void test_slot();

private:
    QPoint curr_pos;
    QPoint click_pos;
    QRect selection;
    QImage img = QImage("/home/johannes/RPG_maker/Pokemon Essentials v19.1 2021-05-22/Graphics/Tilesets/Outside.png");
    QImage img2 = QImage("/home/johannes/RPG_maker/pokemon_decrypted/Graphics/Tilesets/RSEFRLG Complete.png");
};

#endif // TILESETWIDGET_H
