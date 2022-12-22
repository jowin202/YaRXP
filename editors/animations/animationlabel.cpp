#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"
#include "animationlabel.h"

AnimationLabel::AnimationLabel(QWidget *parent) : QLabel(parent)
{
    this->update(0);
}

void AnimationLabel::update(int frame)
{
    QImage img(328,233, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    painter.fillRect(0,0,img.width(),img.height(), Qt::black);


    if (ec != 0)
    {
        QString battler_name = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battler_name").toString();
        int battler_hue = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battler_hue").toInt();



        QImage battler(ec->get_db()->project_dir + "Graphics" + QDir::separator() + "Battlers" + QDir::separator() + battler_name);
        if (!battler.isNull())
        {
            battler = battler.scaled(battler.width()/2, battler.height()/2);//, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

            qreal delta_y = 0;
            int position = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@position").toInt();
            if (position == 0) //top
                delta_y = battler.height()/4.0;
            else if (position == 2) //bottom
                delta_y = -battler.height()/4.0;
            else if (position == 3) //screen
                delta_y = -battler.height()/2.0-img.height()/2.0 + 188;

            //adjust hue
            battler.convertTo(QImage::Format_ARGB32_Premultiplied);
            for (int y = 0; y < battler.height(); y++)
            {
                for (int x = 0; x < battler.width(); x++)
                {
                    QColor col = battler.pixelColor(x,y);
                    if (col.alpha() == 0) continue;
                    col.setHsv(col.hsvHue() + battler_hue,col.saturation(), col.value());
                    battler.setPixelColor(x,y,col);
                }
            }

            painter.drawImage(img.width()/2 - battler.width()/2,qRound(img.height()/2.0 - battler.height()/2.0 + delta_y), battler);
        }
    }



    painter.setPen(QColor(0,0x66,0));
    painter.drawLine(4,36,324,36);
    painter.drawLine(4,36,4,196);
    painter.drawLine(4,196,324,196);
    painter.drawLine(324,36,324,196);

    painter.drawLine(164,36,164,196);
    painter.drawLine(4,116,324,116);


    painter.end();
    this->setPixmap(QPixmap::fromImage(img));
}
