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

        QString animation_name = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@animation_name").toString();
        int animation_hue = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@animation_hue").toInt();
        this->set_animation_graphic(animation_name,animation_hue);



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



    if (ec != 0)
    {
        QJsonArray frames = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
        if (frame >= 0 && frame < frames.count())
        {
            QJsonObject current_frame = frames.at(frame).toObject();
            QJsonObject cell_data = current_frame.value("@cell_data").toObject();
            int cell_max = current_frame.value("@cell_max").toInt();
            QJsonArray values = cell_data.value("values").toArray();

            painter.setPen(Qt::red);
            for (int i = 0; i < cell_max; i++)
            {
                int pattern = values.at(i).toInt();
                int x = values.at(cell_max+i).toInt()/2;
                int y = values.at(2*cell_max+i).toInt()/2;
                int zoom = values.at(3*cell_max+i).toInt();
                //int angle = values.at(4*cell_max+i).toInt();
                int flip = values.at(5*cell_max+i).toInt();
                int opacity = values.at(6*cell_max+i).toInt();
                //int blending = values.at(7*cell_max+i).toInt();

                painter.setOpacity(opacity/255.0);
                int size = qRound(96*zoom/100.0);
                if (!this->animation_graphic.isNull() && pattern >= 0)
                {
                    QImage graphic = animation_graphic.copy(QRect((pattern%5)*96,(pattern/5)*96,95,95));
                    if (flip == 1)
                        graphic.mirror(true,false);
                    painter.drawImage(QRect(img.width()/2-size/2+x,img.height()/2-size/2+y,size-1,size-1),
                                      graphic);
                    //qDebug() << this->animation_graphic.isNull() << animation_graphic.size();
                }
                painter.drawRect(img.width()/2-size/2+x,img.height()/2-size/2+y,size-1,size-1);
            }
        }
    }




    painter.end();
    this->setPixmap(QPixmap::fromImage(img));
}

void AnimationLabel::set_animation_graphic(QString name, int hue)
{
    if (this->current_animation_graphic == name && this->current_hue == hue) return;

    this->animation_graphic = QImage(ec->get_db()->project_dir + "Graphics" + QDir::separator() + "Animations" + QDir::separator() + name);

    if (!this->animation_graphic.isNull())
    {
        this->animation_graphic = animation_graphic.scaled(animation_graphic.width()/2, animation_graphic.height()/2);
        //adjust hue
        animation_graphic.convertTo(QImage::Format_ARGB32_Premultiplied);
        for (int y = 0; y < animation_graphic.height(); y++)
        {
            for (int x = 0; x < animation_graphic.width(); x++)
            {
                QColor col = animation_graphic.pixelColor(x,y);
                if (col.alpha() == 0) continue;
                col.setHsv(col.hsvHue() + hue,col.saturation(), col.value());
                animation_graphic.setPixelColor(x,y,col);
            }
        }

        this->current_hue = hue;
        this->current_animation_graphic = name;
    }

}
