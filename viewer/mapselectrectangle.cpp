#include "mapselectrectangle.h"
#include "mapview.h"

MapSelectRectangle::MapSelectRectangle(RPGMapController *mc,int w, int h)
{
    this->w = w;
    this->h = h;
    this->mc = mc;
}

MapSelectRectangle::~MapSelectRectangle()
{
}

void MapSelectRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (w == 0 || h == 0)
        return; //not displayed if 0


    if (opt != 0 && brush.length() == 3*w*h+2)
    {
        for (int layer = 0; layer < 3; layer++)
        {
            if (layer > opt->layer)
                painter->setOpacity(0.5);
            if (opt->current_and_below && layer > opt->layer) break;

            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    int id1 = this->brush.at(2 + layer*w*h + y*w + x);
                    painter->drawImage(32*QPoint(x,y), this->mc->get_tile_from_id(id1));
                }
            }
        }
    }


    painter->setOpacity(1);
    QPen pen;
    pen.setWidth(4);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->drawRect(2,2,32*w-4,32*h-4);

    pen.setWidth(2);
    pen.setColor(Qt::white);
    pen.setStyle(Qt::PenStyle::DashLine);
    painter->setPen(pen);
    painter->drawRect(2,2,32*w-4,32*h-4);

}

QRectF MapSelectRectangle::boundingRect() const
{
    return QRect(0,0,32*w,32*h);
}

bool MapSelectRectangle::pos_is_in_rectangle(QPoint pos)
{
    if (this->x()/32 > pos.x()) return false;
    if (this->y()/32 > pos.y()) return false;
    if (this->x()/32 + this->w-1 < pos.x()) return false;
    if (this->y()/32 + this->h-1 < pos.y()) return false;

    return true;
}

void MapSelectRectangle::set_brush(QList<int> brush, tile_options *options)
{
    this->brush = brush;
    this->opt = options;
}

void MapSelectRectangle::save_to_map()
{
    QPoint topleft = QPoint(this->x()/32, this->y()/32);
    mc->put_elements_from_list(topleft, QPoint(0,0),this->get_brush(),0,2);
}
