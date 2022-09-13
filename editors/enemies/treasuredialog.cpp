#include "RXIO/RXObjects/rpgsystem.h"
#include "treasuredialog.h"
#include "ui_treasuredialog.h"

TreasureDialog::TreasureDialog(RPGSystem *system, int item_id, int weapon_id, int armor_id, int prob, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreasureDialog)
{
    ui->setupUi(this);

    this->ui->radio_none->setChecked(true);

    system->datasource.fill_combo(this->ui->combo_item, RPGSystem::ITEMS, true, 3, item_id, false);
    system->datasource.fill_combo(this->ui->combo_weapon, RPGSystem::WEAPONS, true, 3, weapon_id, false);
    system->datasource.fill_combo(this->ui->combo_armor, RPGSystem::ARMORS, true, 3, armor_id, false);

    this->ui->radio_item->setChecked(item_id > 0);
    this->ui->radio_weapon->setChecked(weapon_id > 0);
    this->ui->radio_armor->setChecked(armor_id > 0);

    this->ui->spin_probability->setValue(prob);
}

TreasureDialog::~TreasureDialog()
{
    delete ui;
}

void TreasureDialog::on_button_ok_clicked()
{
    int item_id = 0;
    int weapon_id = 0;
    int armor_id = 0;
    if (this->ui->radio_item->isChecked()) item_id = this->ui->combo_item->currentData().toInt();
    if (this->ui->radio_weapon->isChecked()) weapon_id = this->ui->combo_weapon->currentData().toInt();
    if (this->ui->radio_armor->isChecked()) armor_id = this->ui->combo_armor->currentData().toInt();

    emit ok_clicked(item_id, weapon_id, armor_id, this->ui->spin_probability->value());
    this->close();
}

void TreasureDialog::on_button_cancel_clicked()
{
    this->close();
}
