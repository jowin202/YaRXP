#include <QCheckBox>
#include "dialogs/audiodialog.h"
#include "dialogs/imagedialog.h"
#include "editors/plusminusbox.h"

#include "RXIO/RXObjects/rpgaudiofile.h"
#include "RXIO/RXObjects/rpgskill.h"
#include "RXIO/RXObjects/rpgsystem.h"

#include "editskills.h"
#include "ui_editskills.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

EditSkills::EditSkills(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditSkills)
{
    ui->setupUi(this);
}

EditSkills::~EditSkills()
{
    delete ui;
}

void EditSkills::setEC(RPGEditorController *ec)
{
    this->ec = ec;

    this->ec->connect_string_to_text_field(RPGDB::SKILLS, "@name", this->ui->line_name);
    this->ec->connect_string_to_text_field(RPGDB::SKILLS, "@icon_name", this->ui->line_icon);
    this->ec->connect_string_to_text_field(RPGDB::SKILLS, "@description", this->ui->line_description);

    this->ec->connect_int_to_combo_box(RPGDB::SKILLS, "@scope", this->ui->combo_scope);
    this->ec->connect_int_to_combo_box(RPGDB::SKILLS, "@occasion", this->ui->combo_occasion);

    this->ec->connect_int_to_data_combo_box(RPGDB::SKILLS, "@animation1_id", this->ui->combo_user, RPGDB::ANIMATIONS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::SKILLS, "@animation2_id", this->ui->combo_target, RPGDB::ANIMATIONS, true, 3, true);
    this->ec->connect_int_to_data_combo_box(RPGDB::SKILLS, "@common_event_id", this->ui->combo_common_event, RPGDB::COMMONEVENTS, true, 3, true);

    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@sp_cost", this->ui->spin_sp_cost);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@power", this->ui->spin_power);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@atk_f", this->ui->spin_atkf);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@eva_f", this->ui->spin_evaf);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@str_f", this->ui->spin_strf);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@dex_f", this->ui->spin_dexf);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@agi_f", this->ui->spin_agif);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@int_f", this->ui->spin_intf);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@hit", this->ui->spin_hit);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@pdef_f", this->ui->spin_pdef);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@mdef_f", this->ui->spin_mdef);
    this->ec->connect_int_to_spin_box(RPGDB::SKILLS, "@variance", this->ui->spin_variance);

    this->ec->connect_array_to_checkbox_list(RPGDB::SKILLS, "@element_set", this->ui->element_widget, RPGDB::ELEMENTS);
}

void EditSkills::set_skill(int n)
{
    /*
    if (this->system->skills_list.length() <= n) return;
    RPGSkill *current_skill = this->system->skills_list.at(n);
    this->current = n;


    this->ui->line_name->setText(current_skill->name);
    this->ui->line_description->setText(current_skill->description);
    this->ui->line_icon->setText(current_skill->icon_name);

    this->ui->combo_scope->setCurrentIndex(current_skill->scope);
    this->ui->combo_occasion->setCurrentIndex(current_skill->occasion);

    system->datasource.fill_combo(this->ui->combo_user, RPGSystem::ANIMATIONS, true, 3, current_skill->animation1_id, true);
    system->datasource.fill_combo(this->ui->combo_target, RPGSystem::ANIMATIONS, true, 3, current_skill->animation2_id, true);


    system->datasource.fill_combo(this->ui->combo_common, RPGSystem::COMMONEVENTS, true, 3, current_skill->common_event_id, true);

    this->ui->spin_price->setValue(current_skill->sp_cost);
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

    this->ui->state_widget->setStates(this->system, &current_skill->plus_state_set, &current_skill->minus_state_set);
    //this->ui->element_widget->setValues(this->system, &current_skill->element_set, -1); //-1 for elements


    this->ui->line_menu_use->setText(current_skill->menu_se.name);
    current_skill->menu_se.copy_to(&this->menu_se);

    //qDebug() << current_skill->to_hash();
    */
}

void EditSkills::save()
{
    /*
    int n = this->current;
    if (this->system->skills_list.length() <= n) return;
    RPGSkill *current_skill = this->system->skills_list.at(n);

    current_skill->name = this->ui->line_name->text();
    current_skill->description = this->ui->line_description->text();
    current_skill->icon_name = this->ui->line_icon->text();

    current_skill->scope = this->ui->combo_scope->currentIndex();
    current_skill->occasion = this->ui->combo_occasion->currentIndex();

    current_skill->animation1_id = this->ui->combo_user->currentData().toInt();
    current_skill->animation2_id = this->ui->combo_target->currentData().toInt();

    current_skill->common_event_id = this->ui->combo_common->currentData().toInt();


    current_skill->sp_cost = this->ui->spin_price->value();
    current_skill->power = this->ui->spin_power->value();
    current_skill->atk_f = this->ui->spin_atkf->value();
    current_skill->eva_f = this->ui->spin_evaf->value();
    current_skill->str_f = this->ui->spin_strf->value();
    current_skill->dex_f = this->ui->spin_dexf->value();
    current_skill->agi_f = this->ui->spin_agif->value();
    current_skill->int_f = this->ui->spin_intf->value();
    current_skill->hit = this->ui->spin_hit->value();
    current_skill->pdef_f = this->ui->spin_pdef->value();
    current_skill->mdef_f = this->ui->spin_mdef->value();
    current_skill->variance = this->ui->spin_variance->value();

    this->menu_se.copy_to(&current_skill->menu_se);

    this->ui->state_widget->getStates(&current_skill->plus_state_set, &current_skill->minus_state_set);
    //this->ui->element_widget->getValues(&current_skill->element_set);

    //qDebug() << current_skill->to_hash();
    */
}

void EditSkills::on_button_se_clicked()
{
    /*
    AudioDialog *dialog = new AudioDialog(system, &this->menu_se, AudioDialog::SE, 0);
    connect(dialog,SIGNAL(ok_clicked(QString)), this->ui->line_menu_use, SLOT(setText(QString)));
    dialog->show();
    */
}

void EditSkills::on_button_icon_clicked()
{
    /*
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::ICONS, this->ui->line_icon->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_icon, SLOT(setText(QString)));
    dialog->show();
    */
}
