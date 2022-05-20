#include "mapproperties.h"
#include "ui_mapproperties.h"

MapProperties::MapProperties(RPGMapInfo *info, Settings *settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapProperties)
{
    ui->setupUi(this);
    this->mapinfo = info;
    this->ui->widget_bgm->setSettings(settings);
    this->ui->widget_bgs->setSettings(settings);


    this->ui->widget_bgm->setData(AudioWidget::BGM, info->map->bgm);
    this->ui->widget_bgs->setData(AudioWidget::BGS, info->map->bgs);


    this->ui->line_id->setText(QString::number(this->mapinfo->id));
    this->ui->line_name->setText(this->mapinfo->name);
    this->ui->check_auto_change_bgm->setChecked(this->mapinfo->map->autoplay_bgm);
    this->ui->check_auto_change_bgs->setChecked(this->mapinfo->map->autoplay_bgs);




    QHashIterator<int, RPGTileset*> i(settings->tilesets);
    while (i.hasNext()) {
        i.next();
        this->ui->combo_tileset->addItem(i.value()->name, QVariant(i.key()));
    }


    this->ui->spin_x->setValue(info->map->width);
    this->ui->spin_y->setValue(info->map->height);

    //this->ui->combo_tileset;
}

MapProperties::~MapProperties()
{
    qDebug() << "dialog deleted";
    delete ui;
}

void MapProperties::closeEvent(QCloseEvent *event)
{
    this->deleteLater();
}

void MapProperties::on_check_auto_change_bgm_toggled(bool checked)
{
    this->ui->widget_bgm->setEnabled(checked);
}

void MapProperties::on_check_auto_change_bgs_toggled(bool checked)
{
    this->ui->widget_bgs->setEnabled(checked);
}
