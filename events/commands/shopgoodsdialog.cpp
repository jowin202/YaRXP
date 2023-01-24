#include "shopgoodsdialog.h"
#include "ui_shopgoodsdialog.h"

#include "RXIO2/rpgdb.h"
ShopGoodsDialog::ShopGoodsDialog(RPGDB *db, int type, int current, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShopGoodsDialog)
{
    ui->setupUi(this);
    db->fill_combo(this->ui->combo_item, RPGDB::ITEMS, true, 3);
    db->fill_combo(this->ui->combo_weapon, RPGDB::WEAPONS, true, 3);
    db->fill_combo(this->ui->combo_armor, RPGDB::ARMORS, true, 3);

    if (type == 0)
    {
        this->ui->radio_item->setChecked(true);
        this->ui->combo_item->setCurrentIndex(current-1);
    }
    else if (type == 1)
    {
        this->ui->radio_weapon->setChecked(true);
        this->ui->combo_weapon->setCurrentIndex(current-1);
    }
    else if (type == 2)
    {
        this->ui->radio_armor->setChecked(true);
        this->ui->combo_armor->setCurrentIndex(current-1);
    }
}

ShopGoodsDialog::~ShopGoodsDialog()
{
    delete ui;
}

void ShopGoodsDialog::on_button_ok_clicked()
{
    if (this->ui->radio_item->isChecked())
        emit ok_clicked(0,this->ui->combo_item->currentIndex()+1);
    else if (this->ui->radio_weapon->isChecked())
        emit ok_clicked(1,this->ui->combo_weapon->currentIndex()+1);
    else if (this->ui->radio_armor->isChecked())
        emit ok_clicked(2,this->ui->combo_armor->currentIndex()+1);
    this->close();
}


void ShopGoodsDialog::on_button_cancel_clicked()
{
    this->close();
}


