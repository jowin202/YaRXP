#include "battleprocessingdialog.h"
#include "ui_battleprocessingdialog.h"

#include "RXIO2/rpgdb.h"

BattleProcessingDialog::BattleProcessingDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BattleProcessingDialog)
{
    ui->setupUi(this);
    db->fill_combo(this->ui->combo_troop, RPGDB::TROOPS, true, 3);
    this->ui->combo_troop->setCurrentIndex(parameters.at(0).toInt()-1);

    this->ui->check_escape->setChecked(parameters.at(1).toBool());
    this->ui->check_can_lose->setChecked(parameters.at(2).toBool());

}

BattleProcessingDialog::~BattleProcessingDialog()
{
    delete ui;
}

void BattleProcessingDialog::on_button_ok_clicked()
{
    QJsonArray p;
    p.append(this->ui->combo_troop->currentIndex()+1);
    p.append(this->ui->check_escape->isChecked());
    p.append(this->ui->check_can_lose->isChecked());

    emit ok_clicked(p);
    this->close();
}


void BattleProcessingDialog::on_button_cancel_clicked()
{
    this->close();
}

