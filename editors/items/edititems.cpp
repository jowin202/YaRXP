#include "edititems.h"
#include "ui_edititems.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

#include "dialogs/audiodialog.h"
#include "dialogs/imagedialog.h"


EditItems::EditItems(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditItems)
{
    ui->setupUi(this);


}

EditItems::~EditItems()
{
    delete ui;
}

void EditItems::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ec->connect_string_to_text_field(RPGDB::ITEMS, "@name", this->ui->line_name);
    this->ec->connect_string_to_text_field(RPGDB::ITEMS, "@description", this->ui->line_description);
    this->ec->connect_string_to_text_field(RPGDB::ITEMS, "@icon_name", this->ui->line_icon);


    this->ec->connect_int_to_spin_box(RPGDB::ITEMS, "@price", this->ui->spin_price);
    this->ec->connect_int_to_spin_box(RPGDB::ITEMS, "@parameter_points", this->ui->spin_param);

    this->ec->connect_int_to_spin_box(RPGDB::ITEMS, "@recover_hp_rate", this->ui->spin_rcvr_hp_rate);
    this->ec->connect_int_to_spin_box(RPGDB::ITEMS, "@recover_hp", this->ui->spin_rcvr_hp);
    this->ec->connect_int_to_spin_box(RPGDB::ITEMS, "@recover_sp_rate", this->ui->spin_rcvr_sp_rate);
    this->ec->connect_int_to_spin_box(RPGDB::ITEMS, "@recover_sp", this->ui->spin_rcvr_sp);

    this->ec->connect_int_to_spin_box(RPGDB::ITEMS, "@hit", this->ui->spin_hit);
    this->ec->connect_int_to_spin_box(RPGDB::ITEMS, "@pdef_f", this->ui->spin_pdef);
    this->ec->connect_int_to_spin_box(RPGDB::ITEMS, "@mdef_f", this->ui->spin_mdef);
    this->ec->connect_int_to_spin_box(RPGDB::ITEMS, "@variance", this->ui->spin_variance);

    this->ec->connect_int_to_combo_box(RPGDB::ITEMS, "@scope", this->ui->combo_scope);
    this->ec->connect_int_to_combo_box(RPGDB::ITEMS, "@occasion", this->ui->combo_occasion);

    this->ec->connect_int_to_data_combo_box(RPGDB::ITEMS, "@animation1_id", this->ui->combo_user, RPGDB::ANIMATIONS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ITEMS, "@animation2_id", this->ui->combo_target, RPGDB::ANIMATIONS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::ITEMS, "@common_event_id", this->ui->combo_common, RPGDB::COMMONEVENTS, true, 3, true);


    this->ec->connect_int_to_combo_box(RPGDB::ITEMS, "@parameter_type", this->ui->combo_parameter);


    this->ec->connect_bool_to_check_box(RPGDB::ITEMS, "@consumable", this->ui->check_consumable);


    this->ec->connect_array_to_checkbox_list(RPGDB::ITEMS, "@element_set", this->ui->element_widget, RPGDB::ELEMENTS);
    //this->ec->connect_array_to_checkbox_list(RPGDB::ITEMS, "@guard_state_set", this->ui->state_widget, RPGDB::STATES);
    //state is plusminuslist
}

void EditItems::set_item(int n)
{
    /*
    if (this->system->items_list.length() <= n) return;
    RPGItem *current_item = this->system->items_list.at(n);
    this->current = n;

    this->ui->line_name->setText(current_item->name);
    this->ui->line_icon->setText(current_item->icon_name);
    this->ui->line_description->setText(current_item->description);

    this->ui->combo_scope->setCurrentIndex(current_item->scope);
    this->ui->combo_occasion->setCurrentIndex(current_item->occasion);

    system->datasource.fill_combo(this->ui->combo_user, RPGSystem::ANIMATIONS, true, 3, current_item->animation1_id, true);
    system->datasource.fill_combo(this->ui->combo_target, RPGSystem::ANIMATIONS, true, 3, current_item->animation2_id, true);

    current_item->menu_se.copy_to(&this->menu_se);
    this->ui->line_menu_use->setText(current_item->menu_se.name);


    system->datasource.fill_combo(this->ui->combo_common, RPGSystem::COMMONEVENTS, true, 3, current_item->common_event_id, true);

    this->ui->spin_price->setValue(current_item->price);
    this->ui->combo_consumable->setCurrentIndex(current_item->consumable ? 0 : 1 );
    this->ui->combo_parameter->setCurrentIndex(current_item->parameter_type);
    this->ui->spin_param->setValue(current_item->parameter_points);

    this->ui->spin_param->setEnabled(this->ui->combo_parameter->currentIndex() != 0);

    this->ui->spin_rcvr_hp->setValue(current_item->recover_hp);
    this->ui->spin_rcvr_hp_rate->setValue(current_item->recover_hp_rate);
    this->ui->spin_rcvr_sp->setValue(current_item->recover_sp);
    this->ui->spin_rcvr_sp_rate->setValue(current_item->recover_sp_rate);
    this->ui->spin_hit->setValue(current_item->hit);
    this->ui->spin_pdef->setValue(current_item->pdef_f);
    this->ui->spin_mdef->setValue(current_item->mdef_f);
    this->ui->spin_variance->setValue(current_item->variance);

    this->ui->element_widget->setValues(system, &current_item->element_set, -1); //-1 for elements
    this->ui->state_widget->setStates(system, &current_item->plus_state_set, &current_item->minus_state_set);
    */
}

void EditItems::save()
{
    /*
    int n = this->current;
    if (this->system->items_list.length() <= n) return;
    RPGItem *current_item = this->system->items_list.at(n);


    current_item->name = this->ui->line_name->text();
    current_item->icon_name = this->ui->line_icon->text();
    current_item->description = this->ui->line_description->text();

    current_item->scope = this->ui->combo_scope->currentIndex();
    current_item->occasion = this->ui->combo_occasion->currentIndex();

    current_item->animation1_id = this->ui->combo_user->currentData().toInt();
    current_item->animation2_id = this->ui->combo_target->currentData().toInt();

    this->menu_se.copy_to(&current_item->menu_se);
    current_item->common_event_id = this->ui->combo_common->currentData().toInt();


    current_item->price = this->ui->spin_price->value();
    current_item->consumable = (this->ui->combo_consumable->currentIndex() == 0);
    current_item->parameter_type = this->ui->combo_parameter->currentIndex();
    current_item->parameter_points = this->ui->spin_param->value();

    current_item->recover_hp = this->ui->spin_rcvr_hp->value();
    current_item->recover_hp_rate = this->ui->spin_rcvr_hp_rate->value();
    current_item->recover_sp = this->ui->spin_rcvr_sp->value();
    current_item->recover_sp_rate = this->ui->spin_rcvr_sp_rate->value();

    current_item->hit = this->ui->spin_hit->value();
    current_item->pdef_f = this->ui->spin_pdef->value();
    current_item->mdef_f = this->ui->spin_mdef->value();
    current_item->variance = this->ui->spin_variance->value();

    this->ui->element_widget->getValues(&current_item->element_set);
    this->ui->state_widget->getStates(&current_item->plus_state_set, &current_item->minus_state_set);
    */
}

void EditItems::on_combo_parameter_currentIndexChanged(int index)
{
    this->ui->spin_param->setEnabled(index != 0);
}

void EditItems::on_button_se_clicked()
{
    /*
    AudioDialog *dialog = new AudioDialog(system, &this->menu_se, AudioDialog::SE, 0);
    connect(dialog,SIGNAL(ok_clicked(QString)), this->ui->line_menu_use, SLOT(setText(QString)));
    dialog->show();
    */
}


void EditItems::on_button_icon_clicked()
{
    /*
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::ICONS, this->ui->line_icon->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_icon, SLOT(setText(QString)));
    dialog->show();
    */
}
