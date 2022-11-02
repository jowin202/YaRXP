#include "editstates.h"
#include "ui_editstates.h"

#include "RXIO/RXObjects/rpgsystem.h"

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

void EditStates::set_state(int n)
{
    if (this->system->states_list.length() <= n) return;
    RPGState *current_state = this->system->states_list.at(n);
    this->current = n;


    this->ui->line_name->setText(current_state->name);
    system->datasource.fill_combo(this->ui->combo_animation, RPGSystem::ANIMATIONS, true, 3, current_state->animation_id, true);
    this->ui->combo_restriction->setCurrentIndex(current_state->restriction);

    this->ui->check_nonresistance->setChecked(current_state->nonresistance);
    this->ui->check_zero_hp->setChecked(current_state->zero_hp);
    this->ui->check_cant_get_exp->setChecked(current_state->cant_get_exp);
    this->ui->check_cant_evade->setChecked(current_state->cant_evade);
    this->ui->check_slip_damage->setChecked(current_state->slip_damage);

    this->ui->spin_rating->setValue(current_state->rating);
    this->ui->spin_hitrate->setValue(current_state->hit_rate);
    this->ui->spin_maxhp->setValue(current_state->maxhp_rate);
    this->ui->spin_maxsp->setValue(current_state->maxsp_rate);

    this->ui->spin_str->setValue(current_state->str_rate);
    this->ui->spin_dex->setValue(current_state->dex_rate);
    this->ui->spin_agi->setValue(current_state->agi_rate);
    this->ui->spin_int->setValue(current_state->int_rate);

    this->ui->spin_atk->setValue(current_state->atk_rate);
    this->ui->spin_pdef->setValue(current_state->pdef_rate);
    this->ui->spin_mdef->setValue(current_state->mdef_rate);
    this->ui->spin_eva->setValue(current_state->eva);


    this->ui->check_battle->setChecked(current_state->battle_only);
    this->ui->spin_turns->setValue(current_state->hold_turn);

    this->ui->spin_prob1->setValue(current_state->auto_release_prob);
    this->ui->spin_prob2->setValue(current_state->shock_release_prob);

    this->ui->element_widget->setValues(system, &current_state->guard_element_set, -1); //-1 for elements
    this->ui->state_widget->setStates(system, &current_state->plus_state_set, &current_state->minus_state_set);
}

void EditStates::save()
{
    int n = this->current;
    if (this->system->states_list.length() <= n) return;
    RPGState *current_state = this->system->states_list.at(n);




    current_state->name = this->ui->line_name->text();
    current_state->animation_id = this->ui->combo_animation->currentData().toInt();

    current_state->restriction = this->ui->combo_restriction->currentIndex();
    current_state->nonresistance = this->ui->check_nonresistance->isChecked();
    current_state->zero_hp = this->ui->check_zero_hp->isChecked();
    current_state->cant_get_exp = this->ui->check_cant_get_exp->isChecked();
    current_state->cant_evade = this->ui->check_cant_evade->isChecked();
    current_state->slip_damage = this->ui->check_slip_damage->isChecked();


    current_state->rating = this->ui->spin_rating->value();
    current_state->hit_rate = this->ui->spin_hitrate->value();
    current_state->maxhp_rate = this->ui->spin_maxhp->value();
    current_state->maxsp_rate = this->ui->spin_maxsp->value();

    current_state->str_rate = this->ui->spin_str->value();
    current_state->dex_rate = this->ui->spin_dex->value();
    current_state->agi_rate = this->ui->spin_agi->value();
    current_state->int_rate = this->ui->spin_int->value();

    current_state->atk_rate = this->ui->spin_atk->value();
    current_state->pdef_rate = this->ui->spin_pdef->value();
    current_state->mdef_rate = this->ui->spin_mdef->value();
    current_state->eva = this->ui->spin_eva->value();

    current_state->battle_only = this->ui->check_battle->isChecked();
    current_state->hold_turn = this->ui->spin_turns->value();
    current_state->auto_release_prob = this->ui->spin_prob1->value();
    current_state->shock_release_prob = this->ui->spin_prob2->value();


    this->ui->element_widget->getValues(&current_state->guard_element_set); //-1 for elements
    this->ui->state_widget->getStates(&current_state->plus_state_set, &current_state->minus_state_set);


}
