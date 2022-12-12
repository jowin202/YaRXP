#include "trooppageconditiondialog.h"
#include "ui_trooppageconditiondialog.h"


#include "RXIO2/rpgdb.h"
#include "RXIO2/factory.h"
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
    this->page_num = page_num;
    QJsonArray members = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@members").toArray();

    QJsonObject page_condition_object = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray().at(page_num).toObject().value("@condition").toObject();
    //qDebug() << ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray().at(page_num);

    this->ui->check_turn->setChecked(page_condition_object.value("@turn_valid").toBool());
    this->ui->check_enemy->setChecked(page_condition_object.value("@enemy_valid").toBool());
    this->ui->check_actor->setChecked(page_condition_object.value("@actor_valid").toBool());
    this->ui->check_switch->setChecked(page_condition_object.value("@switch_valid").toBool());

    this->ui->spin_turn_a->setValue(page_condition_object.value("@turn_a").toInt());
    this->ui->spin_turn_b->setValue(page_condition_object.value("@turn_b").toInt());

    this->ui->spin_enemy->setValue(page_condition_object.value("@enemy_hp").toInt());
    this->ui->spin_actor->setValue(page_condition_object.value("@actor_hp").toInt());

    ec->fill_combo(this->ui->combo_actor, RPGDB::ACTORS, true, 3, false);
    int index = this->ui->combo_actor->findData(page_condition_object.value("@actor_id").toInt());
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
    this->ui->combo_enemy->setCurrentIndex(page_condition_object.value("@enemy_index").toInt());
    this->ui->widget_switch->setValue(page_condition_object.value("@switch_id").toInt());
}

void TroopPageConditionDialog::on_button_ok_clicked()
{
    QJsonObject page_condition_object = Factory().create_troop_page_condition();
    page_condition_object.insert("@turn_valid", this->ui->check_turn->isChecked());
    page_condition_object.insert("@actor_valid", this->ui->check_actor->isChecked());
    page_condition_object.insert("@enemy_valid", this->ui->check_enemy->isChecked());
    page_condition_object.insert("@switch_valid", this->ui->check_switch->isChecked());

    page_condition_object.insert("@turn_a", this->ui->spin_turn_a->value());
    page_condition_object.insert("@turn_b", this->ui->spin_turn_b->value());


    page_condition_object.insert("@enemy_hp", this->ui->spin_enemy->value());
    page_condition_object.insert("@actor_hp", this->ui->spin_actor->value());

    page_condition_object.insert("@enemy_index", this->ui->combo_enemy->currentIndex()); //current index, not current data
    page_condition_object.insert("@actor_id", this->ui->combo_actor->currentData().toInt());

    page_condition_object.insert("@switch_id", this->ui->widget_switch->getValue());


    QJsonArray page_array = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray();
    QJsonObject current_page = page_array.at(page_num).toObject();
    current_page.insert("@condition", page_condition_object);
    page_array.removeAt(page_num);
    page_array.insert(page_num,current_page);
    ec->obj_set_jsonvalue(RPGDB::TROOPS, "@pages", page_array);


    emit values_changed();
    this->close();
}

void TroopPageConditionDialog::on_button_cancel_clicked()
{
    this->close();
}
