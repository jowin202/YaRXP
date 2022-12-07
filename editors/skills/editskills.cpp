#include <QCheckBox>
#include "dialogs/audiodialog.h"
#include "dialogs/imagedialog.h"
#include "editors/plusminusbox.h"


#include "editskills.h"
#include "ui_editskills.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"
#include "RXIO2/factory.h"

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
    this->ec->connect_plus_minus_list(RPGDB::SKILLS, "@plus_state_set", "@minus_state_set", this->ui->state_widget, RPGDB::STATES);

    connect(this->ec, SIGNAL(current_skill_changed()), this, SLOT(get_menu_se()));
}


void EditSkills::on_button_se_clicked()
{
    QJsonObject se = this->ec->obj_get_jsonvalue(RPGDB::SKILLS, "@menu_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), se.value("@name").toString(), se.value("@volume").toInt(), se.value("@pitch").toInt(), AudioDialog::SE, 0);
    connect(dialog,SIGNAL(ok_clicked(QString,int,int)), this, SLOT(set_menu_se(QString,int,int)));
    dialog->show();
}

void EditSkills::on_button_icon_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::ICONS, this->ui->line_icon->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_icon, SLOT(setText(QString)));
    dialog->show();
}

void EditSkills::get_menu_se()
{
    QJsonObject se = this->ec->obj_get_jsonvalue(RPGDB::SKILLS, "@menu_se").toObject();
    this->ui->line_menu_use->setText(se.value("@name").toString());
}

void EditSkills::set_menu_se(QString name, int volume, int pitch)
{
    this->ec->obj_set_jsonvalue(RPGDB::SKILLS, "@menu_se", Factory().create_audiofile(name, volume, pitch));
    this->get_menu_se();
}
