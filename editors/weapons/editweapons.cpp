#include "editweapons.h"
#include "ui_editweapons.h"

#include "dialogs/imagedialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

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

void EditWeapons::setEC(RPGEditorController *ec)
{
    this->ec = ec;

    this->ec->connect_string_to_text_field(RPGDB::WEAPONS, "@name", this->ui->line_name);
    this->ec->connect_string_to_text_field(RPGDB::WEAPONS, "@description", this->ui->line_description);
    this->ec->connect_string_to_text_field(RPGDB::WEAPONS, "@icon_name", this->ui->line_icon);

    this->ec->connect_int_to_data_combo_box(RPGDB::WEAPONS, "@animation1_id", this->ui->combo_attacker, RPGDB::ANIMATIONS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::WEAPONS, "@animation2_id", this->ui->combo_target, RPGDB::ANIMATIONS, true, 3, true);


    this->ec->connect_int_to_spin_box(RPGDB::WEAPONS, "@price", this->ui->spin_price);
    this->ec->connect_int_to_spin_box(RPGDB::WEAPONS, "@atk", this->ui->spin_atk);
    this->ec->connect_int_to_spin_box(RPGDB::WEAPONS, "@pdef", this->ui->spin_pdef);
    this->ec->connect_int_to_spin_box(RPGDB::WEAPONS, "@mdef", this->ui->spin_mdef);

    this->ec->connect_int_to_spin_box(RPGDB::WEAPONS, "@str_plus", this->ui->spin_str);
    this->ec->connect_int_to_spin_box(RPGDB::WEAPONS, "@dex_plus", this->ui->spin_dex);
    this->ec->connect_int_to_spin_box(RPGDB::WEAPONS, "@agi_plus", this->ui->spin_agi);
    this->ec->connect_int_to_spin_box(RPGDB::WEAPONS, "@int_plus", this->ui->spin_int);

    this->ec->connect_array_to_checkbox_list(RPGDB::WEAPONS, "@element_set", this->ui->element_widget, RPGDB::ELEMENTS);

}

void EditWeapons::set_weapon(int n)
{
    /*
    if (this->system->weapons_list.length() <= n) return;
    RPGWeapon *current_weapon = this->system->weapons_list.at(n);
    this->current = n;

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

    //this->ui->element_widget->setValues(system, &current_weapon->element_set,-1); // -1 for elements
    this->ui->state_widget->setStates(system, &current_weapon->plus_state_set, &current_weapon->minus_state_set);
    */
}

void EditWeapons::save()
{
    /*
    int n = this->current;
    if (this->system->weapons_list.length() <= n) return;
    RPGWeapon *current_weapon = this->system->weapons_list.at(n);


    current_weapon->name = this->ui->line_name->text();
    current_weapon->description = this->ui->line_description->text();
    current_weapon->icon_name = this->ui->line_icon->text();

    current_weapon->animation1_id = this->ui->combo_attacker->currentData().toInt();
    current_weapon->animation2_id = this->ui->combo_target->currentData().toInt();



    current_weapon->price = this->ui->spin_price->value();
    current_weapon->atk = this->ui->spin_atk->value();
    current_weapon->pdef = this->ui->spin_pdef->value();
    current_weapon->mdef = this->ui->spin_mdef->value();

    current_weapon->str_plus = this->ui->spin_str->value();
    current_weapon->dex_plus = this->ui->spin_dex->value();
    current_weapon->agi_plus = this->ui->spin_agi->value();
    current_weapon->int_plus = this->ui->spin_int->value();


    //this->ui->element_widget->getValues(&current_weapon->element_set);
    this->ui->state_widget->getStates(&current_weapon->plus_state_set, &current_weapon->minus_state_set);
    */

}

void EditWeapons::on_button_icon_clicked()
{
    /*
    ImageDialog *dialog = new ImageDialog(db, ImageDialog::ICONS, this->ui->line_icon->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_icon, SLOT(setText(QString)));
    dialog->show();
    */
}
