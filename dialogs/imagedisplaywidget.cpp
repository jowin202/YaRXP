#include "imagedialog.h"
#include "imagechooserdialog.h"
#include "imagedisplaywidget.h"

#include "RXIO2/fileopener.h"

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
    if (mode == ImageDialog::EVENTPAGE)
    {
        if (tile_id == 0)
        {
            QImage tmp = FileOpener(db->character_dir,current_file).get_image();
            pic = tmp.copy(tmp.width()/4*pattern,tmp.height()/4*(direction/2-1),tmp.width()/4, tmp.height()/4);
        }
        else if (tile_id >= 384) // no autotiles allowed here
        {
            QJsonObject tileset = this->db->get_tileset_by_id(tileset_id);
            QImage tileset_img = FileOpener(db->tileset_dir,tileset.value("@tileset_name").toString()).get_image();
            if (!tileset_img.isNull())
            {
                int x = (tile_id-384)%8;
                int y = (tile_id-384)/8;
                pic = tileset_img.copy(32*x,32*y,32,32);
            }
        }
    }
    else if (mode == ImageDialog::CHARACTERS)
    {
        QImage tmp = FileOpener(db->character_dir,current_file).get_image();
        pic = tmp.copy(0,0,tmp.width()/4, tmp.height()/4);
    }
    else if (mode == ImageDialog::BATTLERS)
        pic = FileOpener(db->battler_dir,current_file).get_image();
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

void ImageDisplayWidget::set_data_from_page(RPGDB *db, QString character_name, int character_hue, int pattern, int direction, int opacity, int blend_type, int tile_id, int tileset_id)
{
    this->mode = ImageDialog::EVENTPAGE;
    this->db = db;
    this->current_file = character_name;
    this->hue = character_hue;

    //specific
    this->pattern = pattern;
    this->direction = direction;
    this->opacity = opacity;
    this->blend_type = blend_type;
    this->tile_id = tile_id;
    this->tileset_id = tileset_id;

    this->set_background_color();
    this->update_image();
}

QJsonObject ImageDisplayWidget::get_data_from_page()
{
    return Factory().create_page_graphic(this->current_file, this->hue, this->blend_type, this->direction, this->opacity, this->pattern, this->tile_id);
}

void ImageDisplayWidget::mouseDoubleClickEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::RightButton)
        return;

    if (mode == ImageDialog::EVENTPAGE)
    {
        ImageChooserDialog *dialog = new ImageChooserDialog(db, this->current_file, this->hue,
                                                            this->pattern, this->direction,
                                                            this->opacity, this->blend_type, this->tile_id,
                                                            this->tileset_id);
        connect(dialog, &ImageChooserDialog::ok_clicked,[=](QString current_file, int hue, int pattern, int direction, int opacity, int blend_type, int tile_id){
            this->current_file = current_file;
            this->hue = hue;
            this->pattern = pattern;
            this->direction = direction;
            this->opacity = opacity;
            this->blend_type = blend_type;
            this->tile_id = tile_id;

            this->update_image();
        });
        dialog->show();
    }
    else
    {
        ImageDialog *dialog = new ImageDialog(db, mode, current_file);
        dialog->set_hue(this->hue);
        connect(dialog, SIGNAL(ok_clicked_with_hue(int)), this, SLOT(set_hue(int)));
        connect(dialog, SIGNAL(ok_clicked(QString)), this, SLOT(set_current(QString)));
        dialog->show();
    }
}
