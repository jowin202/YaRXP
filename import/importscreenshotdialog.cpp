#include "importscreenshotdialog.h"
#include "ui_importscreenshotdialog.h"

#include "../RXIO2/rpgdb.h"
#include "../RXIO2/fileopener.h"

#include "screenshotanalysisthread.h"

ImportScreenshotDialog::ImportScreenshotDialog(RPGDB *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportScreenshotDialog)
{
    ui->setupUi(this);
    this->db = db;
    this->db->fill_combo(this->ui->combo_tileset,RPGDB::TILESETS, true, 3);

    this->ui->view_orig->setScene(new QGraphicsScene);
    this->ui->view_orig->setAlignment(Qt::AlignLeft | Qt::AlignTop);


    connect(this->ui->spin_left, &QSpinBox::valueChanged, [=](){ this->load_screenshot(); });
    connect(this->ui->spin_right, &QSpinBox::valueChanged, [=](){ this->load_screenshot(); });
    connect(this->ui->spin_top, &QSpinBox::valueChanged, [=](){ this->load_screenshot(); });
    connect(this->ui->spin_bottom, &QSpinBox::valueChanged, [=](){ this->load_screenshot(); });
    connect(this->ui->spin_scale_x, &QSpinBox::valueChanged, [=](){ this->load_screenshot(); });
    connect(this->ui->spin_scale_y, &QSpinBox::valueChanged, [=](){ this->load_screenshot(); });
    connect(this->ui->radio_scale_width, &QRadioButton::toggled, [=](){ this->load_screenshot(); });
    connect(this->ui->radio_scale_height, &QRadioButton::toggled, [=](){ this->load_screenshot(); });
    connect(this->ui->check_gridlines, &QCheckBox::toggled, [=](){ this->load_screenshot(); });

}

ImportScreenshotDialog::~ImportScreenshotDialog()
{
    delete ui;
}

void ImportScreenshotDialog::on_button_import_clicked()
{

}


void ImportScreenshotDialog::on_combo_tileset_currentIndexChanged(int index)
{

}


void ImportScreenshotDialog::on_button_browse_img_clicked()
{
    QSettings settings;
    QString screenshot_path = settings.value("screenshot_path").toString();

    screenshot_path = QFileDialog::getOpenFileName(this, "Find Map Image", screenshot_path, "Image Files (*.png *.jpg);;");
    if (screenshot_path != "")
        settings.setValue("screenshot_path", screenshot_path);

    this->ui->line_path->setText(screenshot_path);

    this->orig_img = QImage(screenshot_path);
    this->load_screenshot();
}

void ImportScreenshotDialog::load_screenshot()
{
    if (this->orig_img.isNull()) return; //assuming that an image exist

    QImage cropped_image(this->orig_img.width() + this->ui->spin_left->value() + this->ui->spin_right->value(),
                         this->orig_img.height() + this->ui->spin_top->value() + this->ui->spin_bottom->value(),
                         QImage::Format_ARGB32_Premultiplied);
    cropped_image.fill(Qt::black);

    QPainter painter(&cropped_image);
    painter.drawImage(this->ui->spin_left->value(), this->ui->spin_top->value(), this->orig_img);
    painter.end();

    this->ui->label_resolution_px->setText(QString("%1 x %2").arg(cropped_image.width()).arg(cropped_image.height()));
    this->ui->label_resolution_tiles->setText(QString("%1 x %2").arg(cropped_image.width()/32).arg(cropped_image.height()/32));

    //if (this->ui->spin_scale->value() == 100)
    //    this->edited_img = QImage(cropped_image);
    //else
    this->edited_img = QImage(cropped_image).scaled(cropped_image.width()*2.5, cropped_image.height()*2.5, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    /*
    if (this->ui->radio_scale_width->isChecked())
        this->edited_img = cropped_image.scaledToWidth(32*this->ui->spin_scale_x->value(), Qt::SmoothTransformation);
    else
        this->edited_img = cropped_image.scaledToHeight(32*this->ui->spin_scale_y->value(), Qt::SmoothTransformation);
    */

    QImage shown_image = QImage(edited_img); //do not draw gridlines in cached image
    if (this->ui->check_gridlines->isChecked())
    {
        QPainter painter2(&shown_image);
        for (int i = 0; i <= shown_image.width()/32; i++)
            painter2.drawLine(32*i,0,32*i,shown_image.height());
        for (int i = 0; i <= shown_image.height()/32; i++)
            painter2.drawLine(0,32*i,shown_image.width(),32*i);
        painter2.end();
    }

    this->ui->view_orig->scene()->clear();
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(shown_image));
    item->setOpacity(0.5);
    this->ui->view_orig->scene()->addItem(item);
    this->ui->view_orig->scene()->setSceneRect(0,0,shown_image.width(),shown_image.height());
}


void ImportScreenshotDialog::on_button_similartiles_clicked()
{
    ScreenshotAnalysisThread *thread = new ScreenshotAnalysisThread(this->edited_img, 1.0-(1.0*this->ui->slider_threshold->value())/(this->ui->slider_threshold->maximum()));
    connect(thread, &ScreenshotAnalysisThread::finished, [=](QMap<QPair<int,int>, int > result) {
        this->similar_tiles = result;
    });
    thread->start();
}


void ImportScreenshotDialog::on_button_show_similar_tiles_clicked()
{
    for (QPair<int,int> pair : this->similar_tiles.keys())
    {
        QImage img(32,32, QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::transparent);
        QPainter painter;
        painter.begin(&img);
        painter.drawText(0,0,32,32, Qt::AlignCenter, QString::number(this->similar_tiles.value(pair)));
        painter.end();

        QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(img));
        item->setPos(32*pair.first, 32*pair.second);
        this->ui->view_orig->scene()->addItem(item);
    }
}

