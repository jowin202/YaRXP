#include "editweapons.h"
#include "ui_editweapons.h"

#include "RXIO/RXObjects/rpgsystem.h"

EditWeapons::EditWeapons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWeapons)
{
    ui->setupUi(this);
}

EditWeapons::~EditWeapons()
{
    delete ui;
}

void EditWeapons::set_weapon(int n)
{
    if (this->system->weapons_list.length() <= n) return;
    RPGWeapon *current_weapon = this->system->weapons_list.at(n);

    this->ui->line_name->setText(current_weapon->name);
    this->ui->line_description->setText(current_weapon->description);
    this->ui->line_icon->setText(current_weapon->icon_name);

    system->datasource.fill_combo(this->ui->combo_attacker, RPGSystem::ANIMATIONS, true, 3, current_weapon->animation1_id, true);
    system->datasource.fill_combo(this->ui->combo_target, RPGSystem::ANIMATIONS, true, 3, current_weapon->animation2_id, true);

    this->ui->spin_price->setValue(current_weapon->price);
    this->ui->spin_atk->setValue(current_weapon->atk);
    this->ui->spin_pdef->setValue(current_weapon->pdef);
    this->ui->spin_mdef->setValue(current_weapon->mdef);

    this->ui->spin_str->setValue(current_weapon->str_plus);
    this->ui->spin_dex->setValue(current_weapon->dex_plus);
    this->ui->spin_agi->setValue(current_weapon->agi_plus);
    this->ui->spin_int->setValue(current_weapon->int_plus);

    this->ui->element_widget->setValues(system, &current_weapon->element_set,-1); // -1 for elements
    this->ui->state_widget->setStates(system, &current_weapon->plus_state_set, &current_weapon->minus_state_set);
}
