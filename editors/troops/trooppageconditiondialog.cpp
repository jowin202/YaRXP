#include "trooppageconditiondialog.h"
#include "ui_trooppageconditiondialog.h"


#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

TroopPageConditionDialog::TroopPageConditionDialog(RPGEditorController *ec, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TroopPageConditionDialog)
{
    ui->setupUi(this);

    this->ec = ec;
    this->ui->widget_switch->setSwitchWidget(ec->get_db());
}

TroopPageConditionDialog::~TroopPageConditionDialog()
{
    delete ui;
}

void TroopPageConditionDialog::setPage(int page_num, QStringList enemy_list)
{
    qDebug() << ec->get_object_by_id(RPGDB::TROOPS,5);
    //QJsonObject page_object = ec->obj_get_jsonvalue(RPGDB::TROOPS, )
    /*
    this->ui->check_turn->setChecked(n->turn_valid);
    this->ui->check_enemy->setChecked(n->enemy_valid);
    this->ui->check_actor->setChecked(n->actor_valid);
    this->ui->check_switch->setChecked(n->switch_valid);

    this->ui->spin_turn_a->setValue(n->turn_a);
    this->ui->spin_turn_b->setValue(n->turn_b);

    this->ui->spin_enemy->setValue(n->enemy_hp);
    this->ui->spin_actor->setValue(n->actor_hp);

    system->datasource.fill_combo(this->ui->combo_actor, RPGSystem::ACTORS, true, 3, n->actor_id, false);
    */
    //enemy list is different, max 8, troop members
    for (int i = 0; i < enemy_list.length(); i++)
    {
        this->ui->combo_enemy->addItem(QString::number(i+1) + ": " + enemy_list.at(i));
    }

    while(this->ui->combo_enemy->count() < 8)
        this->ui->combo_enemy->addItem(QString::number(this->ui->combo_enemy->count()+1) + ": ");

    //this->ui->combo_enemy->setCurrentIndex(n->enemy_index);


    //this->ui->widget_switch->setValue(n->switch_id);

}

void TroopPageConditionDialog::on_button_ok_clicked()
{

    this->close();
}

void TroopPageConditionDialog::on_button_cancel_clicked()
{
    this->close();
}
