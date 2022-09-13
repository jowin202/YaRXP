#include "editenemies.h"
#include "ui_editenemies.h"

#include "RXIO/RXObjects/rpgsystem.h"

#include "enemyactiondialog.h"
#include "treasuredialog.h"

EditEnemies::EditEnemies(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditEnemies)
{
    ui->setupUi(this);

    for (int i = 2; i <= 10; i++)
        this->ui->table_action->hideColumn(i);
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

    this->setTreasure(current_enemy->item_id,current_enemy->weapon_id,current_enemy->armor_id, current_enemy->treasure_prob);

    this->ui->label_battler_img->setPixmap(QPixmap::fromImage(current_enemy->get_battler_graphics(system)));

    this->ui->element_widget->setElements(system, &current_enemy->element_ranks);
    this->ui->state_widget->setElements(system, &current_enemy->state_ranks);


    this->ui->table_action->clearContents();
    this->ui->table_action->setRowCount(0);
    for (int i = 0; i < current_enemy->action.length(); i++)
    {
        RPGEnemyAction *action = current_enemy->action.at(i);

        QTableWidgetItem *item;
        int row = this->ui->table_action->rowCount();
        this->ui->table_action->setRowCount(row+1);
        this->ui->table_action->setItem(row,0,item = new QTableWidgetItem(action->get_action_as_string(system)));
        this->set_readonly(item);
        this->ui->table_action->setItem(row,1,item = new QTableWidgetItem(action->get_condition_as_string()));
        this->set_readonly(item);

        this->ui->table_action->setItem(row,2,new QTableWidgetItem(QString::number(action->condition_turn_a)));
        this->ui->table_action->setItem(row,3,new QTableWidgetItem(QString::number(action->condition_turn_b)));
        this->ui->table_action->setItem(row,4,new QTableWidgetItem(QString::number(action->condition_hp)));
        this->ui->table_action->setItem(row,5,new QTableWidgetItem(QString::number(action->condition_level)));
        this->ui->table_action->setItem(row,6,new QTableWidgetItem(QString::number(action->condition_switch_id)));
        this->ui->table_action->setItem(row,7,new QTableWidgetItem(QString::number(action->kind)));
        this->ui->table_action->setItem(row,8,new QTableWidgetItem(QString::number(action->skill_id)));
        this->ui->table_action->setItem(row,9,new QTableWidgetItem(QString::number(action->rating)));
        this->ui->table_action->setItem(row,10,new QTableWidgetItem(QString::number(action->basic)));


    }

    this->ui->table_action->resizeColumnsToContents();

}

void EditEnemies::setTreasure(int item_id, int weapon_id, int armor_id, int prob)
{
    QString treasure_name;
    QString percent = QString("%1\% ").arg(prob);

    this->current_item_id = item_id;
    this->current_weapon_id = weapon_id;
    this->current_armor_id = armor_id;
    this->current_prob = prob;

    if (armor_id > 0)
        treasure_name = percent + system->datasource.get_obj_name_by_id(armor_id,RPGSystem::ARMORS, false, 0, false);
    else if (weapon_id > 0)
        treasure_name = percent + system->datasource.get_obj_name_by_id(weapon_id, RPGSystem::WEAPONS, false, 0, false);
    else if (item_id > 0)
        treasure_name = percent + system->datasource.get_obj_name_by_id(item_id, RPGSystem::ITEMS, false, 0, false);
    else
        treasure_name = "(None)";

    this->ui->line_treasure->setText(treasure_name);
}

void EditEnemies::change_action(int row, int turn_a, int turn_b, int hp, int level, int switch_id, int kind, int skill, int rating, int basic)
{
    RPGEnemyAction action;
    action.condition_turn_a = turn_a;
    action.condition_turn_b = turn_b;
    action.condition_hp = hp;
    action.condition_level = level;
    action.condition_switch_id = switch_id;
    action.kind = kind;
    action.skill_id = skill;
    action.basic = basic;

    if (row == -1)
    {
        QTableWidgetItem *item;
        this->ui->table_action->setItem(row,0, item = new QTableWidgetItem(action.get_action_as_string(system)));
        this->set_readonly(item);
        this->ui->table_action->setItem(row,1, item = new QTableWidgetItem(action.get_condition_as_string()));
        this->set_readonly(item);
        this->ui->table_action->setItem(row,2, new QTableWidgetItem(QString::number(turn_a)));
        this->ui->table_action->setItem(row,3, new QTableWidgetItem(QString::number(turn_b)));
        this->ui->table_action->setItem(row,4, new QTableWidgetItem(QString::number(hp)));
        this->ui->table_action->setItem(row,5, new QTableWidgetItem(QString::number(level)));
        this->ui->table_action->setItem(row,6, new QTableWidgetItem(QString::number(switch_id)));
        this->ui->table_action->setItem(row,7, new QTableWidgetItem(QString::number(kind)));
        this->ui->table_action->setItem(row,8, new QTableWidgetItem(QString::number(skill)));
        this->ui->table_action->setItem(row,9, new QTableWidgetItem(QString::number(rating)));
        this->ui->table_action->setItem(row,10, new QTableWidgetItem(QString::number(basic)));
    }
    else
    {
        this->ui->table_action->item(row,0)->setText(action.get_action_as_string(system));
        this->ui->table_action->item(row,1)->setText(action.get_condition_as_string());
        this->ui->table_action->item(row,2)->setText(QString::number(turn_a));
        this->ui->table_action->item(row,3)->setText(QString::number(turn_b));
        this->ui->table_action->item(row,4)->setText(QString::number(hp));
        this->ui->table_action->item(row,5)->setText(QString::number(level));
        this->ui->table_action->item(row,6)->setText(QString::number(switch_id));
        this->ui->table_action->item(row,7)->setText(QString::number(kind));
        this->ui->table_action->item(row,8)->setText(QString::number(skill));
        this->ui->table_action->item(row,9)->setText(QString::number(rating));
        this->ui->table_action->item(row,10)->setText(QString::number(basic));
    }

}

void EditEnemies::on_table_action_itemDoubleClicked(QTableWidgetItem *item)
{
    int turn_a = this->ui->table_action->item(item->row(),2)->text().toInt();
    int turn_b = this->ui->table_action->item(item->row(),3)->text().toInt();
    int hp = this->ui->table_action->item(item->row(),4)->text().toInt();
    int level = this->ui->table_action->item(item->row(),5)->text().toInt();
    int switch_id  = this->ui->table_action->item(item->row(),6)->text().toInt();
    int kind = this->ui->table_action->item(item->row(),7)->text().toInt();
    int skill = this->ui->table_action->item(item->row(),8)->text().toInt();
    int rating = this->ui->table_action->item(item->row(),9)->text().toInt();
    int basic = this->ui->table_action->item(item->row(),10)->text().toInt();
    EnemyActionDialog *dialog = new EnemyActionDialog(system, item->row(), turn_a, turn_b, hp, level, switch_id,kind,skill,rating,basic);
    connect(dialog, SIGNAL(ok_clicked(int,int,int,int,int,int,int,int,int,int)), this, SLOT(change_action(int,int,int,int,int,int,int,int,int,int)));
    dialog->show();
}

void EditEnemies::on_button_action_del_clicked()
{
    this->ui->table_action->removeRow(this->ui->table_action->currentRow());
}

void EditEnemies::on_button_action_add_clicked()
{
    EnemyActionDialog *dialog = new EnemyActionDialog(system, -1, 0, 1, 100, 1, 0,0,1,5,0);
    connect(dialog, SIGNAL(ok_clicked(int,int,int,int,int,int,int,int,int,int)), this, SLOT(change_action(int,int,int,int,int,int,int,int,int,int)));
    dialog->show();
}

void EditEnemies::on_button_change_treasure_clicked()
{
    TreasureDialog *dialog = new TreasureDialog(system,current_item_id,current_weapon_id,current_armor_id,current_prob);
    connect(dialog,SIGNAL(ok_clicked(int,int,int,int)), this, SLOT(setTreasure(int,int,int,int)));
    dialog->show();
}
