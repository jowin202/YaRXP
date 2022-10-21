#include "editarmors.h"
#include "ui_editarmors.h"

#include "dialogs/imagedialog.h"

#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgarmor.h"

EditArmors::EditArmors(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditArmors)
{
    ui->setupUi(this);
}

EditArmors::~EditArmors()
{
    delete ui;
}

void EditArmors::set_armor(int n)
{
    if (this->system->armors_list.length() <= n) return;
    RPGArmor *current_armor = this->system->armors_list.at(n);
    this->current = n;

    this->ui->line_name->setText(current_armor->name);
    this->ui->line_description->setText(current_armor->description);
    this->ui->line_icon->setText(current_armor->icon_name);

    this->ui->combo_kind->setCurrentIndex(current_armor->kind);

    system->datasource.fill_combo(this->ui->combo_auto, RPGSystem::STATES, true, 3, current_armor->auto_state_id, true);

    this->ui->spin_price->setValue(current_armor->price);
    this->ui->spin_pdef->setValue(current_armor->pdef);
    this->ui->spin_mdef->setValue(current_armor->mdef);
    this->ui->spin_eva->setValue(current_armor->eva);

    this->ui->spin_str->setValue(current_armor->str_plus);
    this->ui->spin_dex->setValue(current_armor->dex_plus);
    this->ui->spin_agi->setValue(current_armor->agi_plus);
    this->ui->spin_int->setValue(current_armor->int_plus);

    this->ui->element_widget->setValues(system, &current_armor->guard_element_set,-1); // -1 for elements
    this->ui->state_widget->setValues(system, &current_armor->guard_state_set, RPGSystem::STATES);
}

void EditArmors::save()
{
    int n = this->current;
    if (this->system->armors_list.length() <= n) return;
    RPGArmor *current_armor = this->system->armors_list.at(n);


    current_armor->name = this->ui->line_name->text();
    current_armor->description = this->ui->line_description->text();
    current_armor->icon_name = this->ui->line_icon->text();

    current_armor->kind = this->ui->combo_kind->currentIndex();
    current_armor->auto_state_id = this->ui->combo_auto->currentData().toInt();


    current_armor->price = this->ui->spin_price->value();
    current_armor->pdef = this->ui->spin_pdef->value();
    current_armor->mdef = this->ui->spin_mdef->value();
    current_armor->eva = this->ui->spin_eva->value();

    current_armor->str_plus = this->ui->spin_str->value();
    current_armor->dex_plus = this->ui->spin_dex->value();
    current_armor->agi_plus = this->ui->spin_agi->value();
    current_armor->int_plus = this->ui->spin_int->value();

    this->ui->element_widget->getValues(&current_armor->guard_element_set);
    this->ui->state_widget->getValues(&current_armor->guard_state_set);

}

void EditArmors::on_button_icon_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::ICONS, this->ui->line_icon->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_icon, SLOT(setText(QString)));
    dialog->show();
}
