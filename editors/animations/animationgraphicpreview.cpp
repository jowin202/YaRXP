#include "RXIO2/rpgdb.h"
#include "animationgraphicpreview.h"



AnimationGraphicPreview::AnimationGraphicPreview(QWidget *parent) : QLabel(parent)
{

}

void AnimationGraphicPreview::set_image(QString filename, int hue)
{
    this->filename = filename;
    this->hue = hue;
    this->update();
}

void AnimationGraphicPreview::update()
{
    if (this->db == 0) return;

    QImage img(this->db->project_dir + "Graphics" + QDir::separator() + "Animations" + QDir::separator() + filename);
    if (img.isNull()) return;
    img = img.scaled(img.width()/2, img.height()/2);
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

    int num_images = (img.width()/96) * (img.height()/96);

    QImage bg(192*num_images, 96, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&bg);
    painter.setPen(Qt::black);
    painter.fillRect(0,0,bg.width(),bg.height(),db->transparent);

    int count = 0;
    for (int y = 0; y < img.height()/96; y++)
    {
        for (int x = 0; x < img.width()/96; x++)
        {
            painter.drawImage(96*count,0,img.copy(96*x,96*y,96,96));
            painter.drawLine(96*count,0,96*count,96);
            count++;
        }
    }


    painter.end();
    this->setPixmap(QPixmap::fromImage(bg));
}
