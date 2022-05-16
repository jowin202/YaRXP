#include "autotileset.h"

Autotileset::Autotileset()
{
    this->thumb = QImage(32,32,QImage::Format_ARGB32);
    this->tileset_full = QImage(32,32,QImage::Format_ARGB32);

    QPainter painter1(&thumb);
    painter1.fillRect(0,0,thumb.width(),thumb.height(),Qt::green);
    painter1.end();

    QPainter painter2(&tileset_full);
    painter2.fillRect(0,0,tileset_full.width(),tileset_full.height(),Qt::green);
    painter2.end();

}

Autotileset::Autotileset(QImage tileset)
{
    this->tileset = tileset;
    this->thumb = tileset.copy(QRect(0,0,32,32));

    if (tileset.size().height() == 32)
    {
        this->tileset_full = QImage(256,192, QImage::Format_ARGB32);
        QPainter painter(&tileset_full);
        for (int i = 0; i < 47; i++)
        {
            int x = (i % 8) * 32;
            int y = (i/8)*32;
            painter.drawImage(QRect(x,y,32,32), tileset.copy(QRect(0,0,32,32)), QRect(0,0,32,32));
        }
        painter.end();
    }
    if (tileset.size().height() > 32)
    {
        this->subimage_list.append(tileset.copy(QRect(64,0,16,16)));
        this->subimage_list.append(tileset.copy(QRect(80,0,16,16)));
        this->subimage_list.append(tileset.copy(QRect(64,16,16,16)));
        this->subimage_list.append(tileset.copy(QRect(80,16,16,16)));

        for (int y = 0; y < 6; y++)
            for (int x = 0; x < 6; x++)
                this->subimage_list.append(tileset.copy(QRect(16*x,32+16*y,16,16)));




        this->tileset_full = QImage(256,192, QImage::Format_ARGB32);
        QPainter painter(&this->tileset_full);
        painter.fillRect(QRect(0,0,tileset_full.width(), tileset_full.height()),Qt::black);

        int tile_order[][4] = {
            {18,19,24,25},
            {0,19,24,25},
            {18,1,24,25},
            {0,1,24,25},

            {18,19,24,3},
            {0,19,24,3},
            {18,1,24,3},
            {0,1,24,3},

            {18,19,2,25},
            {0,19,2,25},
            {18,1,2,25},
            {0,1,2,25},

            {18,19,2,3},
            {0,19,2,3},
            {18,1,2,3},
            {0,1,2,3}, //15

            {16,17,22,23},
            {16,1,22,23},
            {16,17,22,3},
            {16,1,22,3}, //19

            {6,7,12,13},
            {6,7,12,3},
            {6,7,2,13},
            {6,7,2,3}, //23

            {20,21,26,27},
            {20,21,2,27},
            {0,21,26,27},
            {0,21,2,27}, //27

            {30,31,36,37},
            {0,31,36,37},
            {30,1,36,37},
            {0,1,36,37}, //31

            {16,21,22,27}, //32
            {6,7,36,37}, // check this
            {4,5,10,11},
            {4,5,10,3},

            {8,9,14,15}, //36
            {8,9,2,15},
            {32,33,38,39},
            {0,33,38,39},

            {28,29,34,35},
            {28,1,34,35},
            {4,9,10,15}, //42
            {4,5,34,35},

            {28,33,34,39},
            {8,9,38,39},
            {4,9,34,39}

                     };

        for (int i = 0; i < 47; i++)
        {
            int x = (i % 8) * 32;
            int y = (i/8)*32;
            painter.drawImage(QRect(x,y,32,32), this->create_tile(tile_order[i][0],tile_order[i][1],tile_order[i][2],tile_order[i][3]), QRect(0,0,32,32));
        }

        painter.end();

    }
}

QImage Autotileset::create_tile(int n1, int n2, int n3, int n4)
{

    QImage tile(32,32,QImage::Format_ARGB32);
    QPainter painter(&tile);

    painter.drawImage(QRect(0,0,16,16), this->subimage_list.at(n1), QRect(0,0,16,16));
    painter.drawImage(QRect(16,0,16,16), this->subimage_list.at(n2), QRect(0,0,16,16));
    painter.drawImage(QRect(0,16,16,16), this->subimage_list.at(n3), QRect(0,0,16,16));
    painter.drawImage(QRect(16,16,16,16), this->subimage_list.at(n4), QRect(0,0,16,16));
    painter.end();
    return tile;

}
