#include "RXIO2/rpgeditorcontroller.h"
#include "RXIO2/rpgdb.h"

#include "treasuredialog.h"
#include "ui_treasuredialog.h"

TreasureDialog::TreasureDialog(RPGEditorController *ec, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreasureDialog)
{
    ui->setupUi(this);

    ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@item_id", this->ui->combo_item, RPGDB::ITEMS, true, 3, true);
    ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@weapon_id", this->ui->combo_weapon, RPGDB::WEAPONS, true, 3, true);
    ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@armor_id", this->ui->combo_armor, RPGDB::ARMORS, true, 3, true);

    ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@treasure_prob", this->ui->spin_probability);

    ec->refresh(RPGDB::ENEMIES);

    //TODO: integrate dialog into main dialog
}

TreasureDialog::~TreasureDialog()
{
    delete ui;
}

void TreasureDialog::on_button_ok_clicked()
{
    /*
    int item_id = 0;
    int weapon_id = 0;
    int armor_id = 0;
    if (this->ui->radio_item->isChecked()) item_id = this->ui->combo_item->currentData().toInt();
    if (this->ui->radio_weapon->isChecked()) weapon_id = this->ui->combo_weapon->currentData().toInt();
    if (this->ui->radio_armor->isChecked()) armor_id = this->ui->combo_armor->currentData().toInt();

    emit ok_clicked(item_id, weapon_id, armor_id, this->ui->spin_probability->value());
    this->close();
    */
}

void TreasureDialog::on_button_cancel_clicked()
{
    this->close();
}
