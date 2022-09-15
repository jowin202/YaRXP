#include "edititems.h"
#include "ui_edititems.h"

#include "dialogs/audiodialog.h"

#include "RXIO/RXObjects/rpgsystem.h"

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

void EditItems::set_item(int n)
{
    if (this->system->items_list.length() <= n) return;
    RPGItem *current_item = this->system->items_list.at(n);

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
}

void EditItems::on_combo_parameter_currentIndexChanged(int index)
{
    this->ui->spin_param->setEnabled(index != 0);
}

void EditItems::on_button_se_clicked()
{
    AudioDialog *dialog = new AudioDialog(system, &this->menu_se, AudioDialog::SE, 0);
    connect(dialog,SIGNAL(ok_clicked(QString)), this->ui->line_menu_use, SLOT(setText(QString)));
    dialog->show();
}
