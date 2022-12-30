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

    //TODO: event list
    //this->ui->list->set_data(ec, &page->list);
    //this->ui->list->import_list();

    //first load the condition, then generate string
    this->update_condition();
}


void TroopEventPage::on_button_condition_clicked()
{
    TroopPageConditionDialog *dialog = new TroopPageConditionDialog(ec);
    dialog->setPage(this->page_num);
    connect(dialog, SIGNAL(values_changed()), this, SLOT(update_condition()));
    dialog->show();
}

void TroopEventPage::update_condition()
{
    QJsonObject page = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@pages").toArray().at(page_num).toObject();
    QJsonObject condition_object = page.value("@condition").toObject();


    //handle changes individually (see below)
    this->ui->combo_span->setCurrentIndex(page.value("@span").toInt());

    //condition

    bool enemy_valid = condition_object.value("@enemy_valid").toBool();
    bool switch_valid = condition_object.value("@switch_valid").toBool();
    bool turn_valid = condition_object.value("@turn_valid").toBool();
    bool actor_valid = condition_object.value("@actor_valid").toBool();
    int switch_id = condition_object.value("@switch_id").toInt();
    int turn_a = condition_object.value("@turn_a").toInt();
    int turn_b = condition_object.value("@turn_b").toInt();
    int enemy_index = condition_object.value("@enemy_index").toInt();
    int enemy_hp = condition_object.value("@enemy_hp").toInt();
    int actor_hp = condition_object.value("@actor_hp").toInt();
    int actor_id = condition_object.value("@actor_id").toInt();

    QString condition;

    if (turn_valid)
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
        QJsonObject member = ec->obj_get_jsonvalue(RPGDB::TROOPS, "@members").toArray().at(enemy_index).toObject();
        QString enemy_name = ec->obj_get_name_list(RPGDB::ENEMIES).at(member.value("@enemy_id").toInt()-1);
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

    if (condition == "")
        condition = "Don't run";
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

