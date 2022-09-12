#include "editenemies.h"
#include "ui_editenemies.h"

#include "RXIO/RXObjects/rpgsystem.h"

#include "enemyactiondialog.h"

EditEnemies::EditEnemies(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditEnemies)
{
    ui->setupUi(this);
}

EditEnemies::~EditEnemies()
{
    delete ui;
}

void EditEnemies::set_enemy(int n)
{
    if (this->system->enemies_list.length() <= n) return;
    RPGEnemy *current_enemy = this->system->enemies_list.at(n);

    this->ui->line_name->setText(current_enemy->name);
    this->ui->spin_maxhp->setValue(current_enemy->maxhp);
    this->ui->spin_maxsp->setValue(current_enemy->maxsp);
    this->ui->spin_str->setValue(current_enemy->str);
    this->ui->spin_dex->setValue(current_enemy->dex);
    this->ui->spin_agi->setValue(current_enemy->agi);
    this->ui->spin_int->setValue(current_enemy->int_var);
    this->ui->spin_atk->setValue(current_enemy->atk);
    this->ui->spin_pdef->setValue(current_enemy->pdef);
    this->ui->spin_mdef->setValue(current_enemy->mdef);
    this->ui->spin_eva->setValue(current_enemy->eva);

    system->datasource.fill_combo(this->ui->comb_attacker, RPGSystem::ANIMATIONS, true, 3, current_enemy->animation1_id,true);
    system->datasource.fill_combo(this->ui->combo_target, RPGSystem::ANIMATIONS, true, 3, current_enemy->animation2_id,true);

    this->ui->spin_exp->setValue(current_enemy->exp);
    this->ui->spin_gold->setValue(current_enemy->gold);

    QString treasure_name;
    QString percent = QString("%1\% ").arg(current_enemy->treasure_prob);

    if (current_enemy->armor_id > 0)
        treasure_name = percent + system->datasource.get_obj_name_by_id(current_enemy->armor_id,RPGSystem::ARMORS, false, 0, false);
    else if (current_enemy->weapon_id > 0)
        treasure_name = percent + system->datasource.get_obj_name_by_id(current_enemy->weapon_id, RPGSystem::WEAPONS, false, 0, false);
    else if (current_enemy->item_id > 0)
        treasure_name = percent + system->datasource.get_obj_name_by_id(current_enemy->item_id, RPGSystem::ITEMS, false, 0, false);
    else
        treasure_name = "(None)";

    this->ui->line_treasure->setText(treasure_name);

    this->ui->label_battler_img->setPixmap(QPixmap::fromImage(current_enemy->get_battler_graphics(system)));

    this->ui->element_widget->setElements(system, &current_enemy->element_ranks);
    this->ui->state_widget->setElements(system, &current_enemy->state_ranks);


    this->ui->table_action->clearContents();
    this->ui->table_action->setRowCount(0);
    for (int i = 0; i < current_enemy->action.length(); i++)
    {
        RPGEnemyAction *action = current_enemy->action.at(i);

        int row = this->ui->table_action->rowCount();
        this->ui->table_action->setRowCount(row+1);
        this->ui->table_action->setItem(row,0,new QTableWidgetItem(system->datasource.get_obj_name_by_id(action->skill_id,RPGSystem::SKILLS,false,0,false)));
        this->ui->table_action->setItem(row,1,new QTableWidgetItem(action->get_condition_as_string()));
        this->ui->table_action->setItem(row,2,new QTableWidgetItem(QString::number(i)));
    }

    this->ui->table_action->resizeColumnsToContents();

}

void EditEnemies::on_table_action_itemDoubleClicked(QTableWidgetItem *item)
{
    int id = this->ui->table_action->item(item->row(),3)->text().toInt();
    //EnemyActionDialog *dialog = new EnemyActionDialog(system, );
    //dialog->show();
}
