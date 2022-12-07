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
    this->ec->connect_plus_minus_list(RPGDB::WEAPONS, "@plus_state_set", "@minus_state_set", this->ui->state_widget, RPGDB::STATES);
}

void EditWeapons::on_button_icon_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::ICONS, this->ui->line_icon->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_icon, SLOT(setText(QString)));
    dialog->show();
}
