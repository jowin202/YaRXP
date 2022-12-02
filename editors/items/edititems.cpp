#include "edititems.h"
#include "ui_edititems.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"
#include "RXIO2/factory.h"

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
    this->ec->connect_plus_minus_box(RPGDB::ITEMS, "@plus_state_set", "@minus_state_set", this->ui->state_widget, RPGDB::STATES);


    connect(this->ec, SIGNAL(current_item_changed()), this, SLOT(get_menu_se()));
}

void EditItems::on_combo_parameter_currentIndexChanged(int index)
{
    this->ui->spin_param->setEnabled(index != 0);
}

void EditItems::on_button_se_clicked()
{
    QJsonObject se = this->ec->obj_get_jsonvalue(RPGDB::ITEMS, "@menu_se").toObject();
    AudioDialog *dialog = new AudioDialog(ec->get_db(), se.value("@name").toString(), se.value("@volume").toInt(), se.value("@pitch").toInt(), AudioDialog::SE, 0);
    connect(dialog,SIGNAL(ok_clicked(QString,int,int)), this, SLOT(set_menu_se(QString,int,int)));
    dialog->show();
}


void EditItems::on_button_icon_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::ICONS, this->ui->line_icon->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_icon, SLOT(setText(QString)));
    dialog->show();
}

void EditItems::get_menu_se()
{
    QJsonObject se = this->ec->obj_get_jsonvalue(RPGDB::ITEMS, "@menu_se").toObject();
    this->ui->line_menu_use->setText(se.value("@name").toString());
}

void EditItems::set_menu_se(QString name, int volume, int pitch)
{
    this->ec->obj_set_jsonvalue(RPGDB::ITEMS, "@menu_se", Factory().create_audiofile(name, volume, pitch));
    this->get_menu_se();
}
