#include "animationlabel.h"

AnimationLabel::AnimationLabel(QWidget *parent) : QLabel(parent)
{
    QImage img(328,233, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&img);
    painter.fillRect(0,0,img.width(),img.height(), Qt::black);

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
