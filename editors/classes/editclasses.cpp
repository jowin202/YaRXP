#include "editclasses.h"
#include "ui_editclasses.h"

#include "skilllearning.h"

#include "RXIO2/factory.h"
#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

EditClasses::EditClasses(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditClasses)
{
    ui->setupUi(this);
}

EditClasses::~EditClasses()
{
    delete ui;
}

void EditClasses::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ui->table_skills->setEC(ec);
    this->ec->connect_string_to_text_field(RPGDB::CLASSES, "@name", this->ui->line_name);
    this->ec->connect_int_to_combo_box(RPGDB::CLASSES, "@position", this->ui->combo_pos);

    this->ec->connect_array_to_checkbox_list(RPGDB::CLASSES, "@weapon_set", this->ui->weapon_widget, RPGDB::WEAPONS);
    this->ec->connect_array_to_checkbox_list(RPGDB::CLASSES, "@armor_set", this->ui->armor_widget, RPGDB::ARMORS);

    this->ec->connect_table_to_abc_list(RPGDB::CLASSES, "@element_ranks", this->ui->element_widget, RPGDB::ELEMENTS);
    this->ec->connect_table_to_abc_list(RPGDB::CLASSES, "@state_ranks", this->ui->state_widget, RPGDB::STATES);

    connect(this->ec, SIGNAL(current_class_changed()), this->ui->table_skills, SLOT(update_skill_from_file()));
}


void EditClasses::on_table_skills_itemDoubleClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    this->ui->table_skills->item_edit();
}


void EditClasses::on_button_skill_add_clicked()
{
    this->ui->table_skills->item_add();
}

void EditClasses::on_button_skill_del_clicked()
{
    this->ui->table_skills->item_delete();
}


