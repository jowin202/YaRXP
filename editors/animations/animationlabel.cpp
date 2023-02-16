#include "RXIO2/rpgdb.h"
#include "RXIO2/fileopener.h"
#include "RXIO2/rpgeditorcontroller.h"
#include "animationlabel.h"
#include "cellpropertiesdialog.h"

AnimationLabel::AnimationLabel(QWidget *parent) : QLabel(parent)
{
    this->update(0);
}

void AnimationLabel::update(int frame)
{
    QImage img(328,233, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    painter.fillRect(0,0,img.width(),img.height(), Qt::black);

    this->current_frame = frame;
    this->current_red_rectangles.clear();

    if (ec != 0)
    {
        QString battler_name = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battler_name").toString();
        int battler_hue = this->ec->obj_get_jsonvalue(RPGDB::SYSTEM, "@battler_hue").toInt();

        QString animation_name = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@animation_name").toString();
        int animation_hue = this->ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@animation_hue").toInt();
        this->set_animation_graphic(animation_name,animation_hue);



        QImage battler = FileOpener(ec->get_db()->battler_dir, battler_name).get_image();
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

        //previous frame blue border
        if (frame >= 1 && frame < frames.count())
        {
            QJsonObject prev_frame = frames.at(frame-1).toObject();
            QJsonObject cell_data = prev_frame.value("@cell_data").toObject();
            int cell_max = prev_frame.value("@cell_max").toInt();
            QJsonArray values = cell_data.value("values").toArray();

            painter.setPen(QColor(0,0x4d,0x80));
            for (int i = 0; i < cell_max; i++)
            {
                int pattern = values.at(i).toInt();
                if (pattern < 0) continue;
                int x = values.at(cell_max+i).toInt()/2;
                int y = values.at(2*cell_max+i).toInt()/2;
                int zoom = values.at(3*cell_max+i).toInt();
                int angle = values.at(4*cell_max+i).toInt();

                int size = qRound(96*zoom/100.0);


                if (angle != 0)
                {
                    qreal scale = (qCos(qAbs(angle)/180.0 * 3.1415926)+qSin(qAbs(angle)/180.0 * 3.1415926));
                    size = qRound(size*scale);
                }

                painter.drawRect(img.width()/2-size/2+x,img.height()/2-size/2+y,size-1,size-1);
            }
        }

        //current frame red border
        if (frame >= 0 && frame < frames.count())
        {
            QJsonObject current_frame = frames.at(frame).toObject();
            QJsonObject cell_data = current_frame.value("@cell_data").toObject();
            int cell_max = current_frame.value("@cell_max").toInt();
            QJsonArray values = cell_data.value("values").toArray();

            for (int i = 0; i < cell_max; i++)
            {
                int pattern = values.at(i).toInt();
                if (pattern < 0) continue;
                int x = values.at(cell_max+i).toInt()/2;
                int y = values.at(2*cell_max+i).toInt()/2;
                int zoom = values.at(3*cell_max+i).toInt();
                int angle = values.at(4*cell_max+i).toInt();
                int flip = values.at(5*cell_max+i).toInt();
                int opacity = values.at(6*cell_max+i).toInt();
                //int blending = values.at(7*cell_max+i).toInt();

                int size = qRound(96*zoom/100.0);
                if (!this->animation_graphic.isNull() && pattern >= 0)
                {
                    QImage graphic = animation_graphic.copy(QRect((pattern%5)*192,(pattern/5)*192,191,191));
                    if (flip == 1)
                        graphic.mirror(true,false);

                    if (angle != 0)
                    {
                        qreal scale = (qCos(qAbs(angle)/180.0 * 3.1415926)+qSin(qAbs(angle)/180.0 * 3.1415926));
                        size = qRound(size*scale);
                        QTransform tr;
                        tr.scale(scale,scale);
                        tr.rotate(-angle);
                        QPixmap pix = QPixmap::fromImage(graphic);
                        graphic = pix.transformed(tr).toImage();
                    }

                    painter.setOpacity(opacity/255.0);
                    painter.drawImage(QRect(img.width()/2-size/2+x,img.height()/2-size/2+y,size-1,size-1),
                                      graphic);
                    painter.setOpacity(1);
                }
                if (i == this->current_rectangle)
                    painter.setPen(Qt::white);
                else
                    painter.setPen(Qt::red);
                QRect rect(img.width()/2-size/2+x,img.height()/2-size/2+y,size-1,size-1);
                painter.drawRect(rect);
                this->current_red_rectangles.insert(i,rect);
            }
        }
    }

    painter.end();
    this->setPixmap(QPixmap::fromImage(img));
}

void AnimationLabel::set_animation_graphic(QString name, int hue)
{
    if (this->current_animation_graphic == name && this->current_hue == hue) return;

    this->animation_graphic = FileOpener(ec->get_db()->animations_dir, name).get_image();

    if (!this->animation_graphic.isNull())
    {
        //this->animation_graphic = animation_graphic.scaled(animation_graphic.width()/2, animation_graphic.height()/2);
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
        this->current_frame = -1;
    }
    else
        this->current_animation_graphic = "";

}

void AnimationLabel::mousePressEvent(QMouseEvent *e)
{
    if (this->current_frame == -1)
        return;

    QMapIterator<int, QRect> i(this->current_red_rectangles);
    while (i.hasNext()) {
        i.next();
        if (i.value().contains(e->pos()))
        {
            this->current_rectangle = i.key();
            this->update(this->current_frame);
            this->is_moving = true;
            return;
        }
    }

    //if reached here, insert pattern
    if (this->current_animation_graphic == "") return; //but only if pattern available
    if (qAbs((e->pos().x()-4)/4 * 8 - 320) > 320)
        return;
    if (qAbs((e->pos().y()-36)/4 * 8 - 160) > 160)
        return;

    x = (e->pos().x()-4)/4 * 8 - 320;
    y = (e->pos().y()-36)/4 * 8 - 160;

    int max_cell = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject().value("@cell_max").toInt();

    QJsonArray cell_values = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject().value("@cell_data").toObject().value("values").toArray();
    cell_values.insert(8*max_cell, 0);   //blending
    cell_values.insert(7*max_cell, 255);   //opacity
    cell_values.insert(6*max_cell, 0);   //flip
    cell_values.insert(5*max_cell, 0);   //angle
    cell_values.insert(4*max_cell, 100); //zoom
    cell_values.insert(3*max_cell,y);
    cell_values.insert(2*max_cell,x);
    cell_values.insert(1*max_cell,pattern);

    QJsonObject cell_data = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject().value("@cell_data").toObject();
    cell_data.insert("values", cell_values);

    QJsonObject thisframe = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject();
    thisframe.insert("@cell_data", cell_data);
    thisframe.insert("@cell_max", max_cell+1);

    QJsonArray frame_array = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
    frame_array.removeAt(current_frame);
    frame_array.insert(current_frame, thisframe);

    ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frame_array);
    this->update(current_frame);
}

void AnimationLabel::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    if (is_moving)
    {
        if (qAbs((e->pos().x()-4)/4 * 8 - 320) <= 320)
            x = (e->pos().x()-4)/4 * 8 - 320;
        if (qAbs((e->pos().y()-36)/4 * 8 - 160) <= 160)
            y = (e->pos().y()-36)/4 * 8 - 160;

        int max_cell = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject().value("@cell_max").toInt();
        QJsonArray cell_values = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject().value("@cell_data").toObject().value("values").toArray();
        cell_values.removeAt(1*max_cell+current_rectangle);
        cell_values.insert(1*max_cell+current_rectangle,x);
        cell_values.removeAt(2*max_cell+current_rectangle);
        cell_values.insert(2*max_cell+current_rectangle,y);

        QJsonObject cell_data = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject().value("@cell_data").toObject();
        cell_data.insert("values", cell_values);

        QJsonObject thisframe = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject();
        thisframe.insert("@cell_data", cell_data);

        QJsonArray frame_array = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
        frame_array.removeAt(current_frame);
        frame_array.insert(current_frame, thisframe);

        ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frame_array);
        this->update(current_frame);
    }
}

void AnimationLabel::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    this->is_moving = false;
}

void AnimationLabel::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (this->current_frame == -1)
        return;

    QMapIterator<int, QRect> i(this->current_red_rectangles);
    while (i.hasNext()) {
        i.next();
        if (i.value().contains(e->pos()))
        {
            this->current_rectangle = i.key();
            int max_cell = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject().value("@cell_max").toInt();
            QJsonArray cell_values = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject().value("@cell_data").toObject().value("values").toArray();
            CellPropertiesDialog *dialog = new CellPropertiesDialog(cell_values, this->current_rectangle, max_cell, max_pattern);
            dialog->show();
            connect(dialog, &CellPropertiesDialog::ok_clicked, [=](QJsonArray cell_values)
            {
                QJsonObject cell_data = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject().value("@cell_data").toObject();
                cell_data.insert("values", cell_values);

                QJsonObject thisframe = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray().at(current_frame).toObject();
                thisframe.insert("@cell_data", cell_data);

                QJsonArray frame_array = ec->obj_get_jsonvalue(RPGDB::ANIMATIONS, "@frames").toArray();
                frame_array.removeAt(current_frame);
                frame_array.insert(current_frame, thisframe);

                ec->obj_set_jsonvalue(RPGDB::ANIMATIONS, "@frames", frame_array);
                this->update(current_frame);
            });
            return;
        }
    }
}
