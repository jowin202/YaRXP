#include "RXIO2/rpgdb.h"
#include "RXIO2/fileopener.h"
#include "animationgraphicpreview.h"



AnimationGraphicPreview::AnimationGraphicPreview(QWidget *parent) : QLabel(parent)
{

}

void AnimationGraphicPreview::set_image(QString filename, int hue)
{
    this->filename = filename;
    this->hue = hue;
    this->current_cell = -1;
    this->update();
}

void AnimationGraphicPreview::update()
{
    if (this->db == 0) return;

    QImage img = FileOpener(this->db->animations_dir, filename).get_image();
    if (img.isNull()) return;
    img = img.scaled(img.width()/2, img.height()/2);
    img.convertTo(QImage::Format_ARGB32_Premultiplied);
    for (int y = 0; y < img.height(); y++)
    {
        for (int x = 0; x < img.width(); x++)
        {
            QColor col = img.pixelColor(x,y);
            if (col.alpha() == 0) continue;
            col.setHsv(col.hsvHue() + hue,col.saturation(), col.value());
            img.setPixelColor(x,y,col);
        }
    }

    int num_images = 5 * (img.height()/96); //5 per line

    QImage bg(96*num_images, 96, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&bg);
    painter.setPen(Qt::black);
    painter.fillRect(0,0,bg.width(),bg.height(),db->transparent);

    int count = 0;
    for (int y = 0; y < img.height()/96; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            if (img.width() >= x*96)
                painter.drawImage(96*count,0,img.copy(96*x,96*y,96,96));
            painter.drawLine(96*count,0,96*count,96);
            if (count == this->current_cell)
            {
                painter.setPen(Qt::white);
                painter.drawRect(96*count,0,95,95);
                painter.setPen(Qt::black);
            }
            count++;
        }
    }


    painter.end();
    this->setPixmap(QPixmap::fromImage(bg));
}

void AnimationGraphicPreview::mousePressEvent(QMouseEvent *e)
{
    this->current_cell = e->pos().x()/96;
    this->update();
    emit cell_chosen(this->current_cell);
}
