#include "imagedialog.h"
#include "ui_imagedialog.h"

#include "RXIO2/rpgdb.h"

ImageDialog::ImageDialog(RPGDB *db, int mode, QString current_image_file, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageDialog)
{
    ui->setupUi(this);

    this->db = db;
    this->mode = mode;
    this->current = current_image_file;

    QPalette palette = this->ui->label_display->palette();
    palette.setColor(QPalette::Window, db->transparent);
    this->ui->label_display->setPalette(palette);



    QString title;
    switch (mode) {
    case EVENTPAGE:
        title = "Graphic";
        folder_name = "Characters";
        visible_hue = true;
        visible_options = true;
        break;
    case ANIMATIONS:
        title = "Animation Graphic";
        folder_name = "Animations";
        visible_hue = true;
        break;
    case AUTOTILES:
        title = "Autotile Graphic";
        folder_name = "Autotiles";
        break;
    case BATTLEBACKS:
        title = "Battleback Graphic";
        folder_name = "Battlebacks";
        break;
    case BATTLERS:
        title = "Battlers Graphic";
        folder_name = "Battlers";
        visible_hue = true;
        break;
    case CHARACTERS:
        title = "Character Graphic";
        folder_name = "Characters";
        visible_hue = true;
        break;
    case FOGS:
        title = "Fog Graphic";
        folder_name = "Fogs";
        visible_hue = true;
        visible_fog_options = true;
        visible_options = true;
        break;
    case ICONS:
        title = "Icon Graphic";
        folder_name = "Icons";
        break;
    case PANOMRAMAS:
        title = "Panorama Graphic";
        folder_name = "Panoramas";
        visible_hue = true;
        break;
    case PICTURES:
        title = "Picture Graphic";
        folder_name = "Pictures";
        break;
    case TILESETS:
        title = "Tileset Graphic";
        folder_name = "Tilesets";
        break;
    case TITLES:
        title = "Title Graphic";
        folder_name = "Titles";
        break;
    case TRANSITIONS:
        title = "Transition Graphic";
        folder_name = "Transitions";
        break;
    case WINDOWSKINS:
        title = "Windowskin Graphic";
        folder_name = "Windowskins";
        break;
    case GAMEOVERS:
        title = "Gameover Graphic";
        folder_name = "Gameovers";
        break;
    }

    this->setWindowTitle(title);

    if (visible_hue == false)
        this->ui->group_hue->setVisible(false);

    if (visible_options == false)
        this->ui->group_options->setVisible(false);

    if (visible_fog_options == false)
    {
        this->ui->label_zoom->setVisible(false);
        this->ui->spin_zoom->setVisible(false);
        this->ui->label_sx->setVisible(false);
        this->ui->label_sy->setVisible(false);
        this->ui->spin_sx->setVisible(false);
        this->ui->spin_sy->setVisible(false);
    }

    this->update_image_list();
}

ImageDialog::~ImageDialog()
{
    delete ui;
}

void ImageDialog::set_hue(int hue)
{
    this->ui->slider_hue->setValue(hue);
    this->update_image();
}

void ImageDialog::set_fog_options(int fog_sx, int fog_sy, int fog_opacity, int fog_zoom, int fog_blend_type)
{
    this->ui->spin_sx->setValue(fog_sx);
    this->ui->spin_sy->setValue(fog_sy);
    this->ui->spin_opacity->setValue(fog_opacity);
    this->ui->spin_zoom->setValue(fog_zoom);
    this->ui->combo_blending->setCurrentIndex(fog_blend_type);
}

void ImageDialog::update_image_list()
{
    this->ui->list->clear();
    this->ui->list->addItem("(None)");

    if (mode == EVENTPAGE)
        this->ui->list->addItem("(Tileset)");

    QDir pics_dir(this->db->project_dir + "Graphics" + QDir::separator() + folder_name);

    QStringList entries = pics_dir.entryList(QDir::Filter::NoFilter, QDir::SortFlag::Name);
    for (QString entry : entries)
    {
        if (entry == "." || entry == "..") continue;
        this->ui->list->addItem(entry);
        if (this->current == entry.chopped(4))
        {
            this->ui->list->setCurrentItem(this->ui->list->item(this->ui->list->count()-1));
        }
    }
}

void ImageDialog::update_image()
{
    QImage new_img = current_img.copy();
    //format cannot be indexed or monochrome
    for (int y = 0; y < new_img.height(); y++)
    {
        for (int x = 0; x < new_img.width(); x++)
        {
            QColor col = new_img.pixelColor(x,y);
            if (col.alpha() == 0) continue;
            col.setHsv(col.hsvHue() + hue,col.saturation(), col.value());
            new_img.setPixelColor(x,y,col);
        }
    }

    this->ui->label_display->setPixmap(QPixmap::fromImage(new_img));
}

void ImageDialog::on_button_ok_clicked()
{

    this->close();
    if (this->mode == FOGS)
        emit ok_clicked_with_fog_options(this->ui->spin_sx->value(),
                                         this->ui->spin_sy->value(),
                                         this->ui->spin_opacity->value(),
                                         this->ui->spin_zoom->value(),
                                         this->ui->combo_blending->currentIndex());

    if (this->ui->list->currentItem() != 0)
    {
        if (this->ui->list->currentRow() == 0)
            emit ok_clicked("");
        else
        {
            if (this->visible_hue)
                emit ok_clicked_with_hue(this->ui->slider_hue->value());
            emit ok_clicked(QString(this->ui->list->currentItem()->text()).chopped(4));   
        }
    }

}

void ImageDialog::on_button_cancel_clicked()
{
    this->close();
}

void ImageDialog::on_list_currentRowChanged(int currentRow)
{
    this->current_img = QImage(db->project_dir + "Graphics" + QDir::separator() + this->folder_name + QDir::separator() + this->ui->list->item(currentRow)->text()).convertToFormat(QImage::Format_ARGB32);
    this->update_image();
}

void ImageDialog::on_slider_hue_valueChanged(int value)
{
    this->hue = value;
    this->update_image();
}

void ImageDialog::on_list_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    this->on_button_ok_clicked();
}

