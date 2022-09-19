#include "imagedialog.h"
#include "ui_imagedialog.h"

#include "RXIO/RXObjects/rpgsystem.h"

ImageDialog::ImageDialog(RPGSystem *system, int mode, QString current_image_file, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageDialog)
{
    ui->setupUi(this);

    this->system = system;
    this->mode = mode;
    this->current = current_image_file;

    QPalette palette = this->ui->label_display->palette();
    palette.setColor(QPalette::Window, system->purple);
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
        folder_name = "Transistions";
        break;
    case WINDOWSKINS:
        title = "Windowskin Graphic";
        folder_name = "Windowskins";
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

void ImageDialog::update_image_list()
{
    this->ui->list->clear();
    this->ui->list->addItem("(None)");

    if (mode == EVENTPAGE)
        this->ui->list->addItem("(Tileset)");

    QDir pics_dir(this->system->graphics_dir + folder_name);

    QDirIterator it(pics_dir);
    while (it.hasNext()) {
        it.next();
        if (it.fileName() == "." || it.fileName() == "..")
            continue;

        this->ui->list->addItem(it.fileName());
        if (this->current == it.fileName().chopped(4))
        {
            this->ui->list->setCurrentItem(this->ui->list->item(this->ui->list->count()-1));
        }
    }
}

void ImageDialog::on_button_ok_clicked()
{

    if (this->ui->list->currentItem() != 0)
    {
        if (this->ui->list->currentRow() != 0)
            emit ok_clicked(QString(this->ui->list->currentItem()->text().chopped(4)));
    }

    this->close();
}

void ImageDialog::on_button_cancel_clicked()
{
    this->close();
}

void ImageDialog::on_list_currentRowChanged(int currentRow)
{
    QImage img(system->graphics_dir + this->folder_name + QDir::separator() + this->ui->list->item(currentRow)->text());
    this->ui->label_display->setPixmap(QPixmap::fromImage(img));
}
