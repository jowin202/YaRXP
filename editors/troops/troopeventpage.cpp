#include "troopeventpage.h"
#include "ui_troopeventpage.h"
#include "trooppageconditiondialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

#include "trooppiclabel.h"

TroopEventPage::TroopEventPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TroopEventPage)
{
    ui->setupUi(this);
}

TroopEventPage::~TroopEventPage()
{
    delete ui;
}

void TroopEventPage::setTroopPage(RPGEditorController *ec, int page_num)
{
    this->ec = ec;
    this->page_num = page_num;

    //TODO: avoid using troop pic label here
    //this->troop_pic_label = label; //for enemies

    //TODO: event list
    //this->ui->list->set_data(ec, &page->list);
    //this->ui->list->import_list();

    QJsonObject page = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray().at(page_num).toObject();
    QJsonObject condition = page.value("@condition").toObject();


    //handle changes individually (see below)
    this->ui->combo_span->setCurrentIndex(page.value("@span").toInt());

    //condition
    this->enemy_valid = condition.value("@enemy_valid").toBool();
    this->switch_valid = condition.value("@switch_valid").toBool();
    this->turn_valid = condition.value("@turn_valid").toBool();
    this->actor_valid = condition.value("@actor_valid").toBool();
    this->switch_id = condition.value("@switch_id").toInt();
    this->turn_a = condition.value("@turn_a").toInt();
    this->turn_b = condition.value("@turn_b").toInt();
    this->enemy_index = condition.value("@enemy_index").toInt();
    this->enemy_hp = condition.value("@enemy_hp").toInt();
    this->actor_hp = condition.value("@actor_hp").toInt();
    this->actor_id = condition.value("@actor_id").toInt();


    //first load the condition, then generate string
    this->update_condition();
}


void TroopEventPage::on_button_condition_clicked()
{
    TroopPageConditionDialog *dialog = new TroopPageConditionDialog(ec);
    dialog->setPage(this->page_num);
    dialog->show();
}

void TroopEventPage::update_condition()
{
    QString condition;

    if (this->turn_valid)
    {
        condition += "Turn ";
        if (turn_a == 0 && turn_b == 0)
            condition += "0";
        else if (turn_a == 0 && turn_b > 0)
            condition += QString("%1X").arg(turn_b);
        else if (turn_a > 0 && turn_b == 0)
            condition += QString("%1").arg(turn_a);
        else if (turn_a > 0 && turn_b > 0)
            condition += QString("%1+%2X").arg(turn_a).arg(turn_b);
    }


    if (enemy_valid)
    {
        if (condition != "") condition += " & ";
        QJsonObject member = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@members").toArray().at(enemy_index-1).toObject();
        QString enemy_name = ec->obj_get_name_list(RPGDB::ENEMIES).at(member.value("@enemy_id").toInt());
        condition += QString("Enemy [%1. %2]'s HP %3\% or below")
                .arg(enemy_index+1).arg(enemy_name).arg(enemy_hp);
    }

    if (actor_valid)
    {
        if (condition != "") condition += " & ";
        condition += QString("Actor [%1]'s HP %2\% or below")
                .arg(ec->obj_get_name_list(RPGDB::ACTORS).at(actor_id-1)) //TODO: make this unshitty
                .arg(actor_hp);

    }

    if (switch_valid)
    {
        if (condition != "") condition += " & ";
        condition += QString("Switch [%1: %2] is ON").arg(switch_id,4,10,QChar('0')).arg(ec->get_db()->get_switch_names().at(switch_id).toString());
    }

    this->ui->line_condition->setText(condition);
}

void TroopEventPage::on_combo_span_currentIndexChanged(int index)
{
    QJsonArray page_array = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray();
    QJsonObject current_page = page_array.at(page_num).toObject();
    current_page.insert("@span", index);
    page_array.removeAt(page_num);
    page_array.insert(page_num,current_page);
    ec->obj_set_jsonvalue(RPGDB::TROOPS, "@pages", page_array);
}

