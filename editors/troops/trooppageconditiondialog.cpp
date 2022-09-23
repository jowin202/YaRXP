#include "trooppageconditiondialog.h"
#include "ui_trooppageconditiondialog.h"


#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgtrooppage.h"

TroopPageConditionDialog::TroopPageConditionDialog(RPGSystem *system, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TroopPageConditionDialog)
{
    ui->setupUi(this);

    this->system = system;
    this->ui->widget_switch->setSwitchWidget(system);
}

TroopPageConditionDialog::~TroopPageConditionDialog()
{
    delete ui;
}

void TroopPageConditionDialog::setPage(RPGTroopPage *page, QStringList enemy_list)
{
    this->ui->check_turn->setChecked(page->turn_valid);
    this->ui->check_enemy->setChecked(page->enemy_valid);
    this->ui->check_actor->setChecked(page->actor_valid);
    this->ui->check_switch->setChecked(page->switch_valid);

    this->ui->spin_turn_a->setValue(page->turn_a);
    this->ui->spin_turn_b->setValue(page->turn_b);

    this->ui->spin_enemy->setValue(page->enemy_hp);
    this->ui->spin_actor->setValue(page->actor_hp);

    system->datasource.fill_combo(this->ui->combo_actor, RPGSystem::ACTORS, true, 3, page->actor_id, false);

    //enemy list is different, max 8, troop members
    for (int i = 0; i < enemy_list.length(); i++)
    {
        this->ui->combo_enemy->addItem(QString::number(i+1) + ": " + enemy_list.at(i));
    }

    while(this->ui->combo_enemy->count() < 8)
        this->ui->combo_enemy->addItem(QString::number(this->ui->combo_enemy->count()+1) + ": ");

    this->ui->combo_enemy->setCurrentIndex(page->enemy_index);


    this->ui->widget_switch->setValue(page->switch_id);

}

void TroopPageConditionDialog::on_button_ok_clicked()
{

    this->close();
}

void TroopPageConditionDialog::on_button_cancel_clicked()
{
    this->close();
}
