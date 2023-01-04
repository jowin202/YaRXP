#include "mappropertiesdialog.h"
#include "ui_mappropertiesdialog.h"

#include "audiodialog.h"

#include "objectcombodialog.h"


#include "RXIO2/rpgdb.h"
#include "RXIO2/factory.h"
#include "RXIO2/rpgmapinfocontroller.h"
#include "RXIO2/rpgmapcontroller.h"


MapPropertiesDialog::MapPropertiesDialog(RPGMapInfoController *mic, int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapPropertiesDialog)
{
    ui->setupUi(this);
    this->mic = mic;
    this->id = id;

    this->ui->line_id->setText(QString::number(id));


    if (mic->id_is_valid(id))
    {
        this->map = mic->get_db()->get_mapfile_by_id(id);

        if (this->map != 0)
        {
            this->ui->line_name->setText(mic->get_name(id));
            this->ui->check_auto_change_bgm->setChecked(map->object().value("@autoplay_bgm").toBool());
            this->ui->check_auto_change_bgs->setChecked(map->object().value("@autoplay_bgs").toBool());
            this->ui->line_bgm->setText(map->object().value("@bgm").toObject().value("@name").toString());
            this->ui->line_bgs->setText(map->object().value("@bgs").toObject().value("@name").toString());

            this->ui->spin_steps->setValue(map->object().value("@encounter_step").toInt());


            this->ui->spin_x->setValue(map->object().value("@width").toInt());
            this->ui->spin_y->setValue(map->object().value("@height").toInt());
        }
        else
        {
            //happens if get_mapfile_by_id() returns zero (file does not exist)
            QMessageBox::information(this, "File does not exist", "No file exists for this map.");
            //should not happen
        }
    }
    else
    {
        this->create_new_map = true;
        //map is created when pressing ok
        //dialog should have the default values
    }

    this->ui->table_encounters->hideColumn(1);
    this->ui->table_encounters->resizeColumnsToContents();
}

MapPropertiesDialog::~MapPropertiesDialog()
{
    delete ui;
}

void MapPropertiesDialog::set_encounter_list_value(int row, int troop)
{
    Q_UNUSED(troop);
    Q_UNUSED(row);
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
    if (this->create_new_map)
    {
        //new map
        bool success = mic->create_map(id);
        if (!success)
        {
            //this solution is not suitable for real time applications
            //if implementing multi user system, determine id here instead of maptreewidget
            QMessageBox::critical(this, "Error creating map", "Map cannot be created");
            return;
        }
    }

    mic->set_name(id, this->ui->line_name->text());

    RPGMapController mc;
    mc.setDB(mic->get_db());
    mc.setMap(id, false);
    mc.set_jsonvalue("@encounter_step", this->ui->spin_steps->value());
    mc.set_jsonvalue("@autoplay_bgm", this->ui->check_auto_change_bgm->isChecked());
    mc.set_jsonvalue("@autoplay_bgs", this->ui->check_auto_change_bgs->isChecked());
    mc.set_size(this->ui->spin_x->value(), this->ui->spin_y->value());


    emit ok_clicked();
    this->close();
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
    Q_UNUSED(item);
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
