#include "mappropertiesdialog.h"
#include "ui_mappropertiesdialog.h"

#include "audiodialog.h"

MapPropertiesDialog::MapPropertiesDialog(RPGMapInfo *info, RPGSystem *system, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapPropertiesDialog)
{
    ui->setupUi(this);
    this->mapinfo = info;
    this->system = system;



    if (info->map != 0) //TODO: load map automatically
    {
        this->ui->spin_x->setValue(info->map->width);
        this->ui->spin_y->setValue(info->map->height);
        system->datasource.fill_combo(this->ui->combo_tileset, RPGSystem::TILESETS, true, 3, mapinfo->map->tileset_id, false);
        this->ui->check_auto_change_bgm->setChecked(this->mapinfo->map->autoplay_bgm);
        this->ui->check_auto_change_bgs->setChecked(this->mapinfo->map->autoplay_bgs);
        this->ui->spin_steps->setValue(info->map->encounter_step);

        mapinfo->map->bgm.copy_to(&this->bgm);
        mapinfo->map->bgs.copy_to(&this->bgs);
    }
    this->ui->line_id->setText(QString::number(this->mapinfo->id));
    this->ui->line_name->setText(this->mapinfo->name);
    this->setWindowTitle("Map Properties - ID: " + QString::number(this->mapinfo->id));

    for (int i = 0; i < info->map->encounter_list.length(); i++)
    {
        int rows = this->ui->table_encounters->rowCount();
        this->ui->table_encounters->setRowCount(rows+1);
        this->ui->table_encounters->setItem(rows,0, new QTableWidgetItem(
                                                system->datasource.get_obj_name_by_id(info->map->encounter_list.at(i),
                                                                                      RPGSystem::TROOPS, true, 3, false)));
        this->ui->table_encounters->setItem(rows,1, new QTableWidgetItem(QString::number(info->map->encounter_list.at(i))));
    }

    this->update_audio();
    this->ui->table_encounters->hideColumn(1);
}

MapPropertiesDialog::~MapPropertiesDialog()
{
    delete ui;
}



void MapPropertiesDialog::on_button_ok_clicked()
{
    this->mapinfo->name = this->ui->line_name->text();
    this->mapinfo->map->tileset_id = this->ui->combo_tileset->currentData().toInt();

    this->mapinfo->map->autoplay_bgm = this->ui->check_auto_change_bgm->isChecked();
    this->mapinfo->map->autoplay_bgs = this->ui->check_auto_change_bgs->isChecked();

    this->mapinfo->map->encounter_step = this->ui->spin_steps->value();
    this->mapinfo->map->encounter_list.clear();

    for (int i = 0; i < this->ui->table_encounters->rowCount(); i++)
        this->mapinfo->map->encounter_list.append(this->ui->table_encounters->item(i,1)->text().toInt());

    this->bgm.copy_to(&this->mapinfo->map->bgm);
    this->bgs.copy_to(&this->mapinfo->map->bgs);

    emit ok_clicked();
    this->close();
}

void MapPropertiesDialog::on_button_cancel_clicked()
{
    this->close();
}

void MapPropertiesDialog::on_button_bgm_clicked()
{
    AudioDialog *dialog = new AudioDialog(system, &this->bgm, AudioDialog::BGM);
    connect(dialog, SIGNAL(ok_clicked()), this, SLOT(update_audio()));
    dialog->show();
}

void MapPropertiesDialog::on_button_bgs_clicked()
{
    AudioDialog *dialog = new AudioDialog(system, &this->bgs, AudioDialog::BGS);
    connect(dialog, SIGNAL(ok_clicked()), this, SLOT(update_audio()));
    dialog->show();
}

void MapPropertiesDialog::update_audio()
{
    this->ui->line_bgm->setText(this->bgm.name);
    this->ui->line_bgs->setText(this->bgs.name);
}


void MapPropertiesDialog::on_table_encounters_itemDoubleClicked(QTableWidgetItem *item)
{
    QStringList troop_names;
    for (int i = 0; i < system->troops_list.length(); i++)
        troop_names << system->datasource.get_obj_name_by_id(i+1,RPGSystem::TROOPS, true, 3, false);

    int row = item->row();

    //TODO: input dialog to change troop
}
