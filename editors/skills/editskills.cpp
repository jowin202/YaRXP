#include <QCheckBox>

#include "editors/plusminusbox.h"

#include "RXIO/RXObjects/rpgskill.h"
#include "RXIO/RXObjects/rpgsystem.h"

#include "editskills.h"
#include "ui_editskills.h"

EditSkills::EditSkills(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditSkills)
{
    ui->setupUi(this);


    this->ui->element_layout->setAlignment(Qt::AlignTop);
    this->ui->state_layout->setAlignment(Qt::AlignTop);

}

EditSkills::~EditSkills()
{
    delete ui;
}

void EditSkills::set_skill(int n)
{
    if (this->system->skills_list.length() <= n) return;
    RPGSkill *current_skill = this->system->skills_list.at(n);


    this->ui->line_name->setText(current_skill->name);
    this->ui->line_description->setText(current_skill->description);
    this->ui->line_icon->setText(current_skill->icon_name);

    this->ui->combo_scope->setCurrentIndex(current_skill->scope);
    this->ui->combo_occasion->setCurrentIndex(current_skill->occasion);

    system->datasource.fill_combo(this->ui->combo_user, RPGSystem::ANIMATIONS, true, 3, current_skill->animation1_id);
    system->datasource.fill_combo(this->ui->combo_target, RPGSystem::ANIMATIONS, true, 3, current_skill->animation2_id);

    this->ui->line_menu_use->setText(current_skill->menu_se.name);

    system->datasource.fill_combo(this->ui->combo_common, RPGSystem::COMMONEVENTS, true, 3, current_skill->common_event_id);

    this->ui->spin_spcost->setValue(current_skill->sp_cost);
    this->ui->spin_power->setValue(current_skill->power);
    this->ui->spin_atkf->setValue(current_skill->atk_f);
    this->ui->spin_evaf->setValue(current_skill->eva_f);
    this->ui->spin_strf->setValue(current_skill->str_f);
    this->ui->spin_dexf->setValue(current_skill->dex_f);
    this->ui->spin_agif->setValue(current_skill->agi_f);
    this->ui->spin_intf->setValue(current_skill->int_f);
    this->ui->spin_hit->setValue(current_skill->hit);
    this->ui->spin_pdef->setValue(current_skill->pdef_f);
    this->ui->spin_mdef->setValue(current_skill->mdef_f);
    this->ui->spin_variance->setValue(current_skill->variance);



    QLayoutItem *item;
    while ( (item = this->ui->element_layout->takeAt(0)) != 0)
        delete item->widget();
    while ( (item = this->ui->state_layout->takeAt(0)) != 0)
        delete item->widget();


    for (int i = 1; i < system->elements.length(); i++)
    {
        QCheckBox *checkbox = new QCheckBox(system->elements.at(i));
        checkbox->setChecked(current_skill->element_set.contains(i));
        this->ui->element_layout->addWidget(checkbox);
    }
    //state and elements works different. state starts counting at 0
    for (int i = 0; i < system->states_list.length(); i++)
    {
        PlusMinusBox *checkbox = new PlusMinusBox(system->states_list.at(i)->name);
        if (current_skill->plus_state_set.contains(i+1))
            checkbox->setPlus();
        else if (current_skill->minus_state_set.contains(i+1))
            checkbox->setMinus();
        this->ui->state_layout->addWidget(checkbox);
    }


}
