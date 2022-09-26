#include "troopeventpage.h"
#include "ui_troopeventpage.h"
#include "trooppageconditiondialog.h"

#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgtrooppage.h"

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

void TroopEventPage::setTroopPage(RPGSystem *system, RPGTroopPage *page, TroopPicLabel *label)
{
    this->system = system;
    this->page = page;
    this->troop_pic_label = label; //for enemies
    this->ui->list->set_data(system, &page->list);
    this->ui->list->fill_list();
    this->ui->combo_span->setCurrentIndex(page->span);

    //condition
    this->enemy_valid = page->enemy_valid;
    this->switch_valid = page->switch_valid;
    this->turn_valid = page->turn_valid;
    this->actor_valid = page->actor_valid;
    this->switch_id = page->switch_id;
    this->turn_a = page->turn_a;
    this->turn_b = page->turn_b;
    this->enemy_index = page->enemy_index;
    this->enemy_hp = page->enemy_hp;
    this->actor_hp = page->actor_hp;
    this->actor_id = page->actor_id;

    //first load the condition, then generate string
    this->update_condition();
}


void TroopEventPage::on_button_condition_clicked()
{
    TroopPageConditionDialog *dialog = new TroopPageConditionDialog(system);
    dialog->setPage(page, this->troop_pic_label->get_enemies_list());
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
        condition += QString("Enemy [%1. %2]'s HP %3\% or below")
                .arg(enemy_index+1).arg(troop_pic_label->get_enemies_list().at(enemy_index)).arg(enemy_hp);
    }

    if (actor_valid)
    {
        if (condition != "") condition += " & ";
        condition += QString("Actor [%1]'s HP %2\% or below")
                .arg(system->datasource.get_obj_name_by_id(actor_id, RPGSystem::ACTORS, false, -1, false))
                .arg(actor_hp);

    }

    if (switch_valid)
    {
        if (condition != "") condition += " & ";
        condition += QString("Switch %1 is ON").arg(system->datasource.get_switch_name(switch_id, switch_id, true, 4,true));
    }

    this->ui->line_condition->setText(condition);
}
