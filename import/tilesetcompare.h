#ifndef TILESETCOMPARE_H
#define TILESETCOMPARE_H

#include <QThread>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QSet>
#include <QImage>

class TilesetCompare : public QThread
{
    Q_OBJECT
public:
    TilesetCompare(QImage tileset1, QImage tileset2, QImage autotiles1, QImage autotiles2, QJsonObject map);
    ~TilesetCompare();

    void run();
    int find_tile_in_tileset(QImage tileset, QImage autotiles, QImage tile);

signals:
    void finished(QJsonObject);
    void progress_start(int);
    void progress(int);

private:
    QImage tileset1;
    QImage tileset2;

    QImage autotiles1;
    QImage autotiles2;
    QJsonObject map;
};

#endif // TILESETCOMPARE_H
