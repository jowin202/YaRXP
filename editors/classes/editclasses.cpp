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

    this->ui->table_skills->hideColumn(2);
    this->ui->table_skills->hideColumn(3);

    this->ui->table_skills->verticalHeader()->setVisible(false);
}

EditClasses::~EditClasses()
{
    delete ui;
}

void EditClasses::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ec->connect_string_to_text_field(RPGDB::CLASSES, "@name", this->ui->line_name);
    this->ec->connect_int_to_combo_box(RPGDB::CLASSES, "@position", this->ui->combo_pos);

    this->ec->connect_array_to_checkbox_list(RPGDB::CLASSES, "@weapon_set", this->ui->weapon_widget, RPGDB::WEAPONS);
    this->ec->connect_array_to_checkbox_list(RPGDB::CLASSES, "@armor_set", this->ui->armor_widget, RPGDB::ARMORS);

    this->ec->connect_table_to_abc_list(RPGDB::CLASSES, "@element_ranks", this->ui->element_widget, RPGDB::ELEMENTS);
    this->ec->connect_table_to_abc_list(RPGDB::CLASSES, "@state_ranks", this->ui->state_widget, RPGDB::STATES);

    connect(this->ec, SIGNAL(current_class_changed()), this, SLOT(update_skill_from_file()));
}

void EditClasses::set_skill_from_dialog(int row, int level, int skill)
{
    QJsonArray learnings = this->ec->obj_get_jsonvalue(RPGDB::CLASSES, "@learnings").toArray();
    learnings.removeAt(row);
    learnings.insert(row, Factory().create_learnings(level,skill));
    this->ec->obj_set_jsonvalue(RPGDB::CLASSES, "@learnings", learnings);

    this->update_skill_from_file();
}

void EditClasses::update_skill_from_file()
{
    this->ui->table_skills->clearContents();
    QJsonArray learnings = this->ec->obj_get_jsonvalue(RPGDB::CLASSES, "@learnings").toArray();
    QStringList skill_names = this->ec->obj_get_name_list(RPGDB::SKILLS);
    int rowCount = learnings.count();
    this->ui->table_skills->setRowCount(rowCount);


    for (int row = 0; row < rowCount; row++)
    {
        int level = learnings.at(row).toObject().value("@level").toInt();
        int skill_id = learnings.at(row).toObject().value("@skill_id").toInt();

        QString skill_name = skill_id <= skill_names.count() ? skill_names.at(skill_id-1) : "";

        this->ui->table_skills->setItem(row,0,new QTableWidgetItem("Lv. " + QString::number(level)));
        this->ui->table_skills->setItem(row,1,new QTableWidgetItem(QString("%1: %2").arg(skill_id,3,10,QChar('0')).arg(skill_name)));
        this->ui->table_skills->setItem(row,2,new QTableWidgetItem(QString::number(level)));
        this->ui->table_skills->setItem(row,3,new QTableWidgetItem(QString::number(skill_id)));

        this->set_readonly(this->ui->table_skills->item(row,0));
        this->set_readonly(this->ui->table_skills->item(row,1));
    }

    this->ui->table_skills->resizeColumnsToContents();
}

void EditClasses::on_table_skills_itemDoubleClicked(QTableWidgetItem *item)
{
    int level = this->ui->table_skills->item(item->row(), 2)->text().toInt();
    int skill = this->ui->table_skills->item(item->row(), 3)->text().toInt();

    SkillLearning *dialog = new SkillLearning(ec,item->row(),level,skill, 0);
    connect(dialog, SIGNAL(ok_clicked(int,int,int)), this, SLOT(set_skill_from_dialog(int,int,int)));
    dialog->show();
}


void EditClasses::on_button_skill_add_clicked()
{
    SkillLearning *dialog = new SkillLearning(ec, this->ui->table_skills->rowCount(), 1, 1, 0);
    connect(dialog, SIGNAL(ok_clicked(int,int,int)), this, SLOT(set_skill_from_dialog(int,int,int)));
    dialog->show();
}

void EditClasses::on_button_skill_del_clicked()
{
    int row = this->ui->table_skills->currentRow();
    QJsonArray learnings = this->ec->obj_get_jsonvalue(RPGDB::CLASSES, "@learnings").toArray();
    learnings.removeAt(row);
    this->ec->obj_set_jsonvalue(RPGDB::CLASSES, "@learnings", learnings);

    this->update_skill_from_file();
}
