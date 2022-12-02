#include "editarmors.h"
#include "ui_editarmors.h"

#include "dialogs/imagedialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"


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

void EditArmors::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ec->connect_string_to_text_field(RPGDB::ARMORS, "@name", this->ui->line_name);
    this->ec->connect_string_to_text_field(RPGDB::ARMORS, "@description", this->ui->line_description);
    this->ec->connect_string_to_text_field(RPGDB::ARMORS, "@icon_name", this->ui->line_icon);


    this->ec->connect_int_to_combo_box(RPGDB::ARMORS, "@kind", this->ui->combo_kind);


    this->ec->connect_int_to_data_combo_box(RPGDB::ARMORS, "@auto_state_id", this->ui->combo_auto, RPGDB::STATES, true, 3, true);

    this->ec->connect_int_to_spin_box(RPGDB::ARMORS, "@price", this->ui->spin_price);
    this->ec->connect_int_to_spin_box(RPGDB::ARMORS, "@pdef", this->ui->spin_pdef);
    this->ec->connect_int_to_spin_box(RPGDB::ARMORS, "@mdef", this->ui->spin_mdef);
    this->ec->connect_int_to_spin_box(RPGDB::ARMORS, "@eva", this->ui->spin_eva);
    this->ec->connect_int_to_spin_box(RPGDB::ARMORS, "@str_plus", this->ui->spin_str);
    this->ec->connect_int_to_spin_box(RPGDB::ARMORS, "@dex_plus", this->ui->spin_dex);
    this->ec->connect_int_to_spin_box(RPGDB::ARMORS, "@agi_plus", this->ui->spin_agi);
    this->ec->connect_int_to_spin_box(RPGDB::ARMORS, "@int_plus", this->ui->spin_int);

    this->ec->connect_array_to_checkbox_list(RPGDB::ARMORS, "@guard_element_set", this->ui->element_widget, RPGDB::ELEMENTS);
    this->ec->connect_array_to_checkbox_list(RPGDB::ARMORS, "@guard_state_set", this->ui->state_widget, RPGDB::STATES);
}

void EditArmors::on_button_icon_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::ICONS, this->ui->line_icon->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_icon, SLOT(setText(QString)));
    dialog->show();
}
