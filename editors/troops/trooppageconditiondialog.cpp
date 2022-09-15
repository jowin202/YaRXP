#include "trooppageconditiondialog.h"
#include "ui_trooppageconditiondialog.h"


#include "RXIO/RXObjects/rpgsystem.h"

TroopPageConditionDialog::TroopPageConditionDialog(RPGSystem *system, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TroopPageConditionDialog)
{
    ui->setupUi(this);

    this->ui->widget_switch->setSwitchWidget(system);
    system->datasource.fill_combo(this->ui->combo_actor, RPGSystem::ACTORS, true, 3, -1,false); //TODO:Current
    //enemy list is different, max 8, troop members
}

TroopPageConditionDialog::~TroopPageConditionDialog()
{
    delete ui;
}

void TroopPageConditionDialog::on_button_ok_clicked()
{

    this->close();
}

void TroopPageConditionDialog::on_button_cancel_clicked()
{
    this->close();
}
