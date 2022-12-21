#include "editenemies.h"
#include "ui_editenemies.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/factory.h"
#include "RXIO2/rpgeditorcontroller.h"

#include "dialogs/imagedialog.h"
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

void EditEnemies::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ui->table_action->setEC(ec);

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


    connect(this->ec, SIGNAL(current_enemy_changed()), this->ui->table_action, SLOT(update_actions_from_file()));
}



void EditEnemies::on_table_action_itemDoubleClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    this->ui->table_action->item_edit();
}

void EditEnemies::on_button_action_del_clicked()
{
    this->ui->table_action->item_delete();
}

void EditEnemies::on_button_action_add_clicked()
{
    this->ui->table_action->item_add();
}





