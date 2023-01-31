#include "imagechooserdialog.h"
#include "ui_imagechooserdialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/fileopener.h"

ImageChooserDialog::ImageChooserDialog(RPGDB *db, QString character_name, int character_hue, int pattern, int direction, int opacity, int blend_type, int tile_id, int tileset_id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageChooserDialog)
{
    ui->setupUi(this);
    this->db = db;
    this->character_name = character_name;
    this->ui->slider_hue->setValue(character_hue);
    this->pattern = pattern;
    this->direction = direction;
    this->ui->spin_opacity->setValue(opacity);
    this->ui->combo_blending->setCurrentIndex(blend_type);
    this->tile_id = tile_id;
    this->tileset_id = tileset_id;

    if (tileset_id == 0) //called from moveroute
    {
        this->ui->groupBox->setVisible(false);
    }


    this->ui->graphicsView->setScene(new QGraphicsScene);
    this->ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    this->ui->graphicsView->setBackgroundBrush(db->transparent);


    this->update_image_list();

    if (tile_id != 0)
    {
        int x = (tile_id-384)%8;
        int y = (tile_id-384)/8;
        this->ui->list->setCurrentRow(1);
        this->ui->graphicsView->rectangle->setPos(32*x,32*y);
        this->ui->graphicsView->centerOn(this->ui->graphicsView->rectangle);
    }

    connect(this->ui->graphicsView, &CustomGraphicsView::clicked, [=](int x, int y)
    {
        if (this->ui->list->currentRow() == 1 && tileset_id > 0)
        {
            this->tile_id = 384 + 8*y + x;
        }
        else if (x < 4 && y < 4)
        {
            this->pattern = x;
            this->direction = 2*(y+1);
        }
    });
    connect(this->ui->graphicsView, SIGNAL(doubleclicked()), this, SLOT(on_button_ok_clicked()));
}

ImageChooserDialog::~ImageChooserDialog()
{
    delete ui;
}

void ImageChooserDialog::update_image_list()
{
    this->ui->list->clear();
    this->ui->list->addItem("(None)");

    if (tileset_id > 0)
        this->ui->list->addItem("(Tileset)");


    QDir pics_dir(this->db->character_dir);

    QStringList entries = pics_dir.entryList(QDir::Filter::Files | QDir::Filter::NoDotAndDotDot, QDir::SortFlag::Name);
    for (QString entry : entries)
    {
        this->ui->list->addItem(entry);
        if (this->character_name == entry.chopped(4))
        {
            this->ui->list->setCurrentItem(this->ui->list->item(this->ui->list->count()-1));
        }
    }
}

void ImageChooserDialog::update_image()
{
    if (this->ui->list->currentRow() == 0)
    {
        this->ui->graphicsView->scene()->clear();
        this->ui->graphicsView->scene()->setSceneRect(0,0,32,32);
        this->ui->graphicsView->rectangle = new Rectangle(32, 32, false);
        this->ui->graphicsView->rectangle->setPos(0,0);
        this->ui->graphicsView->scene()->addItem(this->ui->graphicsView->rectangle);
    }
    else if(this->ui->list->currentRow() == 1 && tileset_id > 0)
    {
        //tileset
        QJsonObject current = db->get_tileset_by_id(tileset_id);
        QImage img = FileOpener(db->tileset_dir, current.value("@tileset_name").toString()).get_image();

        img = img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        //format cannot be indexed or monochrome
        for (int y = 0; y < img.height(); y++)
        {
            for (int x = 0; x < img.width(); x++)
            {
                QColor col = img.pixelColor(x,y);
                if (col.alpha() == 0) continue;
                col.setHsv(col.hsvHue() + this->ui->slider_hue->value(),col.saturation(), col.value());
                img.setPixelColor(x,y,col);
            }
        }

        if (!img.isNull())
        {
            QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap::fromImage(img));
            background->setPos(0,0);
            this->ui->graphicsView->scene()->clear();
            this->ui->graphicsView->scene()->addItem(background);
            this->ui->graphicsView->scene()->setSceneRect(0,0,img.width(),img.height());
            this->ui->graphicsView->rectangle = new Rectangle(32, 32, true);
            this->ui->graphicsView->rectangle->setPos(0,0);
            this->ui->graphicsView->scene()->addItem(this->ui->graphicsView->rectangle);
            this->ui->graphicsView->centerOn(this->ui->graphicsView->rectangle);
        }

    }
    else // not zero and not tileset
    {
        //images
        this->character_name = this->ui->list->currentItem()->text();
        QImage img = FileOpener(db->character_dir, character_name).get_image();

        img = img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        //format cannot be indexed or monochrome
        for (int y = 0; y < img.height(); y++)
        {
            for (int x = 0; x < img.width(); x++)
            {
                QColor col = img.pixelColor(x,y);
                if (col.alpha() == 0) continue;
                col.setHsv(col.hsvHue() + this->ui->slider_hue->value(),col.saturation(), col.value());
                img.setPixelColor(x,y,col);
            }
        }

        if (!img.isNull())
        {
            QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap::fromImage(img));
            background->setPos(0,0);
            this->ui->graphicsView->scene()->clear();
            this->ui->graphicsView->scene()->addItem(background);
            this->ui->graphicsView->scene()->setSceneRect(0,0,img.width(),img.height());
            this->ui->graphicsView->rectangle = new Rectangle(img.width()/4, img.height()/4, false);
            this->ui->graphicsView->rectangle->setPos(img.width()/4 * pattern, img.height()/4 * (direction/2-1));
            this->ui->graphicsView->scene()->addItem(this->ui->graphicsView->rectangle);
            //this->centerOn(rectangle);
        }
    }
}

void ImageChooserDialog::on_button_ok_clicked()
{
    if (this->ui->list->currentRow() == 0)
    {
        this->tile_id = 0;
        this->character_name = "";
    }
    else if (this->ui->list->currentRow() == 1 && tileset_id > 0)
    {
        this->character_name = "";
    }
    else //not tileset and not zero
        this->tile_id = 0;

    QString name = QString(character_name);
    name.chop(4);
    this->close();
    emit ok_clicked(name,
                    this->ui->slider_hue->value(),
                    pattern,
                    direction,
                    this->ui->spin_opacity->value(),
                    this->ui->combo_blending->currentIndex(), tile_id);
}


void ImageChooserDialog::on_button_cancel_clicked()
{
    this->close();
}


void ImageChooserDialog::on_list_currentRowChanged(int currentRow)
{
    Q_UNUSED(currentRow);
    this->update_image();
}



void ImageChooserDialog::on_slider_hue_sliderMoved(int position)
{
    Q_UNUSED(position);
    this->update_image();
}

