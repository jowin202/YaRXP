#include "mappropertiesdialog.h"
#include "ui_mappropertiesdialog.h"

#include "audiodialog.h"

#include "objectcombodialog.h"


#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapinfocontroller.h"
#include "RXIO2/rpgmapcontroller.h"


MapPropertiesDialog::MapPropertiesDialog(RPGMapInfoController *mic, int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapPropertiesDialog)
{
    ui->setupUi(this);

    if (!mic->id_is_valid(id))
    {
        this->close();
        return;
    }

    this->ui->line_name->setText(mic->get_name(id));
    RPGMapController mc;
    mc.setDB(mic->get_db());
    mc.setMap(id);
    this->ui->check_auto_change_bgm->setChecked(mc.get_jsonvalue("@autoplay_bgm").toBool());
    this->ui->check_auto_change_bgs->setChecked(mc.get_jsonvalue("@autoplay_bgs").toBool());
    this->ui->line_bgm->setText(mc.get_jsonvalue("@bgm").toObject().value("@name").toString());
    this->ui->line_bgs->setText(mc.get_jsonvalue("@bgs").toObject().value("@name").toString());

    this->ui->spin_steps->setValue(mc.get_jsonvalue("@encounter_step").toInt());


    this->ui->spin_x->setValue(mc.get_width());
    this->ui->spin_y->setValue(mc.get_height());


    /*
    system->datasource.fill_combo(this->ui->combo_tileset, RPGSystem::TILESETS, true, 3, mapinfo->map->tileset_id, false);
    this->ui->check_auto_change_bgm->setChecked(this->mapinfo->map->autoplay_bgm);
    this->ui->check_auto_change_bgs->setChecked(this->mapinfo->map->autoplay_bgs);
    */
    /*
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

        this->set_encounter_list_value(rows, info->map->encounter_list.at(i));
    }

    this->ui->line_bgm->setText(this->bgm.name);
    this->ui->line_bgs->setText(this->bgs.name);
    */

    this->ui->table_encounters->hideColumn(1);
    this->ui->table_encounters->resizeColumnsToContents();
}

MapPropertiesDialog::~MapPropertiesDialog()
{
    delete ui;
}

void MapPropertiesDialog::set_encounter_list_value(int row, int troop)
{
    /*
    QTableWidgetItem *item;
    this->ui->table_encounters->setItem(row,0, item = new QTableWidgetItem(
                                            system->datasource.get_obj_name_by_id(troop,
                                                                                  RPGSystem::TROOPS, true, 3, false)));
    this->set_readonly(item);
    this->ui->table_encounters->setItem(row,1, item = new QTableWidgetItem(QString::number(troop)));
    this->set_readonly(item);
    */
}



void MapPropertiesDialog::on_button_ok_clicked()
{
    /*
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
    */
}

void MapPropertiesDialog::on_button_cancel_clicked()
{
    this->close();
}

void MapPropertiesDialog::on_button_bgm_clicked()
{
    //AudioDialog *dialog = new AudioDialog(system, &this->bgm, AudioDialog::BGM);
    //connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_bgm, SLOT(setText(QString)));
    //dialog->show();
}

void MapPropertiesDialog::on_button_bgs_clicked()
{
    //AudioDialog *dialog = new AudioDialog(system, &this->bgs, AudioDialog::BGS);
    //connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_bgs, SLOT(setText(QString)));
    //dialog->show();
}



void MapPropertiesDialog::update_troop(int val)
{
    if (this->last_edited_troop_row >= 0)
    {
        this->set_encounter_list_value(this->last_edited_troop_row, val);
    }
    else
    {
        int rows = this->ui->table_encounters->rowCount();
        this->ui->table_encounters->setRowCount(rows+1);

        this->set_encounter_list_value(rows, val);
    }

    this->ui->table_encounters->resizeColumnsToContents();
}


void MapPropertiesDialog::on_table_encounters_itemDoubleClicked(QTableWidgetItem *item)
{
    /*
    this->last_edited_troop_row = item->row();
    int current = this->ui->table_encounters->item(item->row(), 1)->text().toInt();

    ObjectComboDialog *dialog = new ObjectComboDialog;
    dialog->setText("Troop:");
    dialog->fillCombo(system, RPGSystem::TROOPS, true, 3, current, false);
    connect(dialog, SIGNAL(ok_clicked(int)), this, SLOT(update_troop(int)));
    dialog->show();
    */
}


void MapPropertiesDialog::on_button_add_clicked()
{
    /*
    this->last_edited_troop_row = -1;
    int current = -1;

    ObjectComboDialog *dialog = new ObjectComboDialog;
    dialog->setText("Troop:");
    dialog->fillCombo(system, RPGSystem::TROOPS, true, 3, current, false);
    connect(dialog, SIGNAL(ok_clicked(int)), this, SLOT(update_troop(int)));
    dialog->show();
    */
}

void MapPropertiesDialog::on_button_del_clicked()
{
    this->ui->table_encounters->removeRow(this->ui->table_encounters->currentRow());
}
