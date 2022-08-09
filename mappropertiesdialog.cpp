#include "mappropertiesdialog.h"
#include "ui_mappropertiesdialog.h"

MapPropertiesDialog::MapPropertiesDialog(RPGMapInfo *info, RPGSettings *settings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapPropertiesDialog)
{
    ui->setupUi(this);
    this->mapinfo = info;
    this->ui->widget_bgm->setSettings(settings);
    this->ui->widget_bgs->setSettings(settings);


    this->ui->widget_bgm->setData(AudioWidget::BGM, &info->map->bgm);
    this->ui->widget_bgs->setData(AudioWidget::BGS, &info->map->bgs);


    this->ui->line_id->setText(QString::number(this->mapinfo->id));
    this->ui->line_name->setText(this->mapinfo->name);
    this->ui->check_auto_change_bgm->setChecked(this->mapinfo->map->autoplay_bgm);
    this->ui->check_auto_change_bgs->setChecked(this->mapinfo->map->autoplay_bgs);




    for (int i = 0; i < settings->tileset_list.length(); i++)
    {
        this->ui->combo_tileset->addItem(QString::number(settings->tileset_list.at(i)->id).rightJustified(3,'0') + ": " + settings->tileset_list.at(i)->name, settings->tileset_list.at(i)->id);
        if (settings->tileset_list.at(i)->id == mapinfo->map->tileset_id)
            this->ui->combo_tileset->setCurrentIndex(i);
    }


    this->ui->spin_x->setValue(info->map->width);
    this->ui->spin_y->setValue(info->map->height);

}

MapPropertiesDialog::~MapPropertiesDialog()
{
    //qDebug() << "dialog deleted";
    delete ui;
}

void MapPropertiesDialog::closeEvent(QCloseEvent *event)
{
    this->deleteLater();
}

void MapPropertiesDialog::on_check_auto_change_bgm_toggled(bool checked)
{
    this->ui->widget_bgm->setEnabled(checked);
}

void MapPropertiesDialog::on_check_auto_change_bgs_toggled(bool checked)
{
    this->ui->widget_bgs->setEnabled(checked);
}

void MapPropertiesDialog::on_button_ok_clicked()
{
    this->mapinfo->name = this->ui->line_name->text();
    this->mapinfo->map->width = this->ui->spin_x->value();
    this->mapinfo->map->height = this->ui->spin_y->value();
    this->mapinfo->map->tileset_id = this->ui->combo_tileset->currentData().toInt();

    this->mapinfo->map->autoplay_bgm = this->ui->check_auto_change_bgm->isChecked();
    this->mapinfo->map->autoplay_bgs = this->ui->check_auto_change_bgs->isChecked();

    this->ui->widget_bgm->save_data_to_object();
    this->ui->widget_bgs->save_data_to_object();

    this->close();

}

void MapPropertiesDialog::on_button_cancel_clicked()
{
    this->close();
}
