#include "imagedialog.h"
#include "imagedisplaywidget.h"


#include "RXIO2/rpgdb.h"


ImageDisplayWidget::ImageDisplayWidget(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);
    this->setAutoFillBackground(true);
}

void ImageDisplayWidget::set_background_color()
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, db->transparent);
    this->setPalette(palette);
}

void ImageDisplayWidget::update_image()
{
    QImage pic;
    /*
    if (mode == ImageDialog::EVENTPAGE && this->page != 0)
        pic = page->get_page_graphic(system);
    else
    */
    if (mode == ImageDialog::CHARACTERS)
    {
        QImage tmp(db->project_dir + "Graphics" + QDir::separator() + "Characters" + QDir::separator() + current_file);
        pic = tmp.copy(0,0,tmp.width()/4, tmp.height()/4);
    }
    else if (mode == ImageDialog::BATTLERS)
        pic = QImage(db->project_dir + "Graphics" + QDir::separator() + "Battlers" + QDir::separator() + current_file);
    pic = pic.convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < pic.height(); y++)
    {
        for (int x = 0; x < pic.width(); x++)
        {
            QColor col = pic.pixelColor(x,y);
            if (col.alpha() == 0) continue;
            col.setHsv(col.hsvHue() + hue,col.saturation(), col.value());
            pic.setPixelColor(x,y,col);
        }
    }

    this->setPixmap(QPixmap::fromImage(pic));
}

void ImageDisplayWidget::set_data(RPGDB *db, int mode, QString current_file)
{
    this->db = db;
    this->mode = mode;
    this->current_file = current_file;

    this->set_background_color();
    this->update_image();
}

void ImageDisplayWidget::set_data(RPGDB *db, int mode, QString current_file, int hue)
{
    this->db = db;
    this->mode = mode;
    this->current_file = current_file;
    this->hue = hue;


    this->set_background_color();
    this->update_image();
}
/*
void ImageDisplayWidget::set_data_from_page(RPGDB *db, RPGEventPage *page)
{
    this->db = db;
    this->mode = ImageDialog::EVENTPAGE;
    this->current_file = page->character_name;
    this->hue = page->character_hue;
    this->tile_id = page->tile_id;
    this->page = page;


    this->set_background_color();
    this->update_image();
}
*/

void ImageDisplayWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::RightButton)
        return;

    ImageDialog *dialog = new ImageDialog(db, mode, current_file);
    dialog->set_hue(this->hue);
    connect(dialog, SIGNAL(ok_clicked_with_hue(int)), this, SLOT(set_hue(int)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this, SLOT(set_current(QString)));
    dialog->show();
}
