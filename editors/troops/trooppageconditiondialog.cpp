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

void TroopPageConditionDialog::setPage(int page_num)
{
    QJsonArray members = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@members").toArray();

    QJsonObject page_object = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray().at(page_num).toObject().value("@condition").toObject();

    this->ui->check_turn->setChecked(page_object.value("@turn_valid").toBool());
    this->ui->check_enemy->setChecked(page_object.value("@enemy_valid").toBool());
    this->ui->check_actor->setChecked(page_object.value("@actor_valid").toBool());
    this->ui->check_switch->setChecked(page_object.value("@switch_valid").toBool());

    this->ui->spin_turn_a->setValue(page_object.value("@turn_a").toInt());
    this->ui->spin_turn_b->setValue(page_object.value("@turn_b").toInt());

    this->ui->spin_enemy->setValue(page_object.value("@enemy_hp").toInt());
    this->ui->spin_actor->setValue(page_object.value("@actor_hp").toInt());

    ec->fill_combo(this->ui->combo_actor, RPGDB::ACTORS, true, 3, false);
    int index = this->ui->combo_actor->findData(page_object.value("@actor_id").toInt());
    if (index != -1)
        this->ui->combo_actor->setCurrentIndex(index);


    //enemy list is different, max 8, troop members
    for (int i = 0; i < members.count(); i++)
    {
        QJsonObject enemy = ec->get_object_by_id(RPGDB::ENEMIES,members.at(i).toObject().value("@enemy_id").toInt());
        this->ui->combo_enemy->addItem(QString::number(i+1) + ": " + enemy.value("@name").toString());
    }
    while(this->ui->combo_enemy->count() < 8)
        this->ui->combo_enemy->addItem(QString::number(this->ui->combo_enemy->count()+1) + ": ");
    this->ui->combo_enemy->setCurrentIndex(page_object.value("@enemy_index").toInt());
    this->ui->widget_switch->setValue(page_object.value("@switch_id").toInt());
}

void TroopPageConditionDialog::on_button_ok_clicked()
{

    this->close();
}

void TroopPageConditionDialog::on_button_cancel_clicked()
{
    this->close();
}
