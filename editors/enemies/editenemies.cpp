#include "editenemies.h"
#include "ui_editenemies.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

#include "dialogs/imagedialog.h"
#include "enemyactiondialog.h"

EditEnemies::EditEnemies(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditEnemies)
{
    ui->setupUi(this);

    /*
    for (int i = 5; i <= 10; i++)
        this->ui->table_action->hideColumn(i);
        */
}

EditEnemies::~EditEnemies()
{
    delete ui;
}

void EditEnemies::setEC(RPGEditorController *ec)
{
    this->ec = ec;

    this->ec->connect_string_to_text_field(RPGDB::ENEMIES, "@name", this->ui->line_name);

    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@maxhp", this->ui->spin_maxhp);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@maxsp", this->ui->spin_maxsp);

    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@str", this->ui->spin_str);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@dex", this->ui->spin_dex);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@agi", this->ui->spin_agi);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@int", this->ui->spin_int);

    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@atk", this->ui->spin_atk);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@pdef", this->ui->spin_pdef);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@mdef", this->ui->spin_mdef);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@eva", this->ui->spin_eva);

    this->ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@animation1_id", this->ui->combo_attacker, RPGDB::ANIMATIONS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@animation2_id", this->ui->combo_target, RPGDB::ANIMATIONS, true, 3, true);

    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@exp", this->ui->spin_exp);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@gold", this->ui->spin_gold);

    this->ec->connect_table_to_abc_list(RPGDB::ENEMIES, "@element_ranks", this->ui->element_widget, RPGDB::ELEMENTS);
    this->ec->connect_table_to_abc_list(RPGDB::ENEMIES, "@state_ranks", this->ui->state_widget, RPGDB::STATES);

    this->ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@armor_id", this->ui->combo_armor, RPGDB::ARMORS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@item_id", this->ui->combo_item, RPGDB::ITEMS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ENEMIES, "@weapon_id", this->ui->combo_weapon, RPGDB::WEAPONS, true, 3, true);
    this->ec->connect_int_to_spin_box(RPGDB::ENEMIES, "@treasure_prob", this->ui->spin_prob);

    this->ec->connect_image_display_widget(RPGDB::ENEMIES, ImageDialog::BATTLERS, "@battler_name", "@battler_hue", this->ui->label_battler_img);


    connect(this->ec, SIGNAL(current_enemy_changed()), this, SLOT(update_actions_from_file()));

}

void EditEnemies::update_actions_from_file()
{
    this->ui->table_action->clearContents();
    QJsonArray actions = this->ec->obj_get_jsonvalue(RPGDB::ENEMIES, "@actions").toArray();
    QStringList skill_names = this->ec->obj_get_name_list(RPGDB::SKILLS);
    int rowCount = actions.count();
    this->ui->table_action->setRowCount(rowCount);


    for (int row = 0; row < rowCount; row++)
    {
        int condition_turn_a = actions.at(row).toObject().value("@condition_turn_a").toInt();
        int condition_turn_b = actions.at(row).toObject().value("@condition_turn_b").toInt();
        int condition_hp = actions.at(row).toObject().value("@condition_hp").toInt();
        int condition_level = actions.at(row).toObject().value("@condition_level").toInt();
        int condition_switch_id = actions.at(row).toObject().value("@condition_switch_id").toInt();

        int kind = actions.at(row).toObject().value("@kind").toInt();
        int rating = actions.at(row).toObject().value("@rating").toInt();
        int skill_id = actions.at(row).toObject().value("@skill_id").toInt();
        int basic = actions.at(row).toObject().value("@basic").toInt();

        QString condition_turns = "";
        if (condition_turn_a == 0 && condition_turn_b == 1) condition_turns = "";
        else if (condition_turn_a == 0 && condition_turn_b == 0) condition_turns = "Turn 0";
        else if (condition_turn_a == 0 && condition_turn_b > 1) condition_turns = QString("Turn %1X").arg(condition_turn_b);
        else if (condition_turn_a > 0 && condition_turn_b == 0) condition_turns = QString("Turn %1").arg(condition_turn_a);
        else condition_turns = QString("Turn %1+%2X").arg(condition_turn_a).arg(condition_turn_b);


        this->ui->table_action->setItem(row,1,new QTableWidgetItem(condition_turns));
        if (condition_hp < 100)
            this->ui->table_action->setItem(row,2,new QTableWidgetItem(QString("%1\% HP or below").arg(condition_hp)));

        if (condition_level > 1)
            this->ui->table_action->setItem(row,3,new QTableWidgetItem(QString("Level %1 or above").arg(condition_level)));

        if (condition_switch_id > 0)
            this->ui->table_action->setItem(row,4,new QTableWidgetItem(QString("Switch [%1] is ON").arg(condition_switch_id,4,10,QChar('0'))));


        this->ui->table_action->setItem(row,5,new QTableWidgetItem(QString::number(condition_turn_a)));
        this->ui->table_action->setItem(row,6,new QTableWidgetItem(QString::number(condition_turn_b)));
        this->ui->table_action->setItem(row,7,new QTableWidgetItem(QString::number(condition_hp)));
        this->ui->table_action->setItem(row,8,new QTableWidgetItem(QString::number(condition_level)));
        this->ui->table_action->setItem(row,9,new QTableWidgetItem(QString::number(condition_switch_id)));
        this->ui->table_action->setItem(row,10,new QTableWidgetItem(QString::number(kind)));
        this->ui->table_action->setItem(row,11,new QTableWidgetItem(QString::number(skill_id)));
        this->ui->table_action->setItem(row,12,new QTableWidgetItem(QString::number(rating)));
        this->ui->table_action->setItem(row,13,new QTableWidgetItem(QString::number(basic)));


    }

    this->ui->table_action->resizeColumnsToContents();
}


void EditEnemies::change_action(int row, int turn_a, int turn_b, int hp, int level, int switch_id, int kind, int skill, int rating, int basic)
{
    /*
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
        row = this->ui->table_action->rowCount();
        this->ui->table_action->setRowCount(row+1);
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
    */
}

void EditEnemies::on_table_action_itemDoubleClicked(QTableWidgetItem *item)
{
    /*
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
    */
}

void EditEnemies::on_button_action_del_clicked()
{
    //this->ui->table_action->removeRow(this->ui->table_action->currentRow());
}

void EditEnemies::on_button_action_add_clicked()
{
    /*
    EnemyActionDialog *dialog = new EnemyActionDialog(system, -1, 0, 1, 100, 1, 0,0,1,5,0);
    connect(dialog, SIGNAL(ok_clicked(int,int,int,int,int,int,int,int,int,int)), this, SLOT(change_action(int,int,int,int,int,int,int,int,int,int)));
    dialog->show();
    */
}

