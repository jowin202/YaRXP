#include "editstates.h"
#include "ui_editstates.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

EditStates::EditStates(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditStates)
{
    ui->setupUi(this);
}

EditStates::~EditStates()
{
    delete ui;
}

void EditStates::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ec->connect_string_to_text_field(RPGDB::STATES, "@name", this->ui->line_name);
    this->ec->connect_int_to_data_combo_box(RPGDB::STATES, "@animation_id", this->ui->combo_animation, RPGDB::ANIMATIONS, true, 3, true);

    this->ec->connect_int_to_combo_box(RPGDB::STATES, "@restriction", this->ui->combo_restriction);

    this->ec->connect_bool_to_check_box(RPGDB::STATES, "@nonresistance", this->ui->check_nonresistance);
    this->ec->connect_bool_to_check_box(RPGDB::STATES, "@zero_hp", this->ui->check_zero_hp);
    this->ec->connect_bool_to_check_box(RPGDB::STATES, "@cant_get_exp", this->ui->check_cant_get_exp);
    this->ec->connect_bool_to_check_box(RPGDB::STATES, "@cant_evade", this->ui->check_cant_evade);
    this->ec->connect_bool_to_check_box(RPGDB::STATES, "@slip_damage", this->ui->check_slip_damage);

    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@rating", this->ui->spin_rating);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@hit_rate", this->ui->spin_hitrate);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@maxhp_rate", this->ui->spin_maxhp);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@maxsp_rate", this->ui->spin_maxsp);

    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@str_rate", this->ui->spin_str);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@dex_rate", this->ui->spin_dex);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@int_rate", this->ui->spin_agi);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@agi_rate", this->ui->spin_int);

    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@atk_rate", this->ui->spin_atk);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@pdef_rate", this->ui->spin_pdef);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@mdef_rate", this->ui->spin_mdef);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@eva", this->ui->spin_eva);


    this->ec->connect_bool_to_check_box(RPGDB::STATES, "@battle_only", this->ui->check_battle);

    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@hold_turn", this->ui->spin_turns);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@auto_release_prob", this->ui->spin_prob1);
    this->ec->connect_int_to_spin_box(RPGDB::STATES, "@shock_release_prob", this->ui->spin_prob2);

    this->ec->connect_array_to_checkbox_list(RPGDB::STATES, "@guard_element_set", this->ui->element_widget, RPGDB::ELEMENTS);
    this->ec->connect_plus_minus_list(RPGDB::STATES, "@plus_state_set", "@minus_state_set",this->ui->state_widget, RPGDB::STATES);
}

