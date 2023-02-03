#include "tilesetcompare.h"

TilesetCompare::TilesetCompare(QImage tileset1, QImage tileset2, QImage autotiles1, QImage autotiles2, QJsonObject map)
{
    this->tileset1 = tileset1;
    this->tileset2 = tileset2;

    this->autotiles1 = autotiles1;
    this->autotiles2 = autotiles2;

    this->map = map;
}

TilesetCompare::~TilesetCompare()
{
}

void TilesetCompare::run()
{
    QJsonObject map_data = map.value("@data").toObject();
    QJsonArray values = map_data.value("values").toArray();

    QSet<int> id_collection;
    for (int i = 0; i < values.count(); i++)
        id_collection.insert(values.at(i).toInt());
    emit progress_start(id_collection.count());

    QMap<int,int> id_transform;

    foreach (const int value, id_collection)
    {
        QImage tile;
        if (value >= 0x180)
        {
            int id = value - 0x180;
            int x = id % 8;
            int y = id / 8;
            tile = tileset2.copy(32*x,32*y,32,32);
        }
        else if (value >= 48)
        {
            int id = value;
            int x = id % 8;
            int y = id / 8;
            tile = autotiles2.copy(32*x,32*y,32,32);
        }
        else continue;

        int new_id = this->find_tile_in_tileset(tileset1, autotiles1, tile);
        if (new_id >= 0)
            id_transform.insert(value,new_id);
        emit progress(id_transform.size());
    }


    for (int i = 0; i < values.count(); i++)
    {
        if (values.at(i).toInt() == 0) continue;

        int v = values.at(i).toInt();
        if (id_transform.contains(v))
        {
            values.removeAt(i);
            values.insert(i,id_transform.value(v));
        }
    }

    map_data.insert("values", values);
    this->map.insert("@data",map_data);

    emit progress(id_collection.count());
    emit has_result(this->map);
}

int TilesetCompare::find_tile_in_tileset(QImage tileset, QImage autotiles, QImage tile)
{
    int best_id = -1;
    int min_diff = 1024 * 255 * 4;


    for (int y = 0; y < tileset.height()/32; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int diff = 0;
            for (int yi = 0; yi < 32; yi++)
            {
                for (int xi = 0; xi < 32; xi++)
                {
                    QRgb rgb1 = tileset.pixel(32*x + xi,32*y + yi);
                    QRgb rgb2 = tile.pixel(xi,yi);

                    diff += (qAlpha(rgb2)/255.0)*(qAbs(qRed(rgb1) - qRed(rgb2)) + qAbs(qGreen(rgb1) - qGreen(rgb2)) + qAbs(qBlue(rgb1) - qBlue(rgb2)))+ qAbs(qAlpha(rgb1) - qAlpha(rgb2));
                }
            }
            if (diff < min_diff)
            {
                min_diff = diff;
                best_id = 0x180 + 8*y+x;
            }
        }
    }


    for (int y = 0; y < autotiles.height()/32; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int diff = 0;
            for (int yi = 0; yi < 32; yi++)
            {
                for (int xi = 0; xi < 32; xi++)
                {
                    QRgb rgb1 = autotiles.pixel(32*x + xi,32*y + yi);
                    QRgb rgb2 = tile.pixel(xi,yi);

                    diff += (qAlpha(rgb2)/255.0)*(qAbs(qRed(rgb1) - qRed(rgb2)) + qAbs(qGreen(rgb1) - qGreen(rgb2)) + qAbs(qBlue(rgb1) - qBlue(rgb2)))+ qAbs(qAlpha(rgb1) - qAlpha(rgb2));
                }
            }
            if (diff < min_diff)
            {
                min_diff = diff;
                best_id = 8*y+x;
            }
        }
    }


    return best_id;
}
