#include "editclasses.h"
#include "ui_editclasses.h"

#include "skilllearning.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

EditClasses::EditClasses(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditClasses)
{
    ui->setupUi(this);

    this->ui->table_skills->hideColumn(2);
    this->ui->table_skills->hideColumn(3);
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
}

void EditClasses::set_class(int n)
{
    /*
    if (this->system->classes_list.length() <= n) return;
    RPGClass *current_class = system->classes_list.at(n);
    this->current = n;

    this->ui->line_name->setText(current_class->name);
    this->ui->combo_pos->setCurrentIndex(current_class->position);


    //this->ui->weapon_widget->setValues(system, &current_class->weapon_set, RPGSystem::WEAPONS);
    //this->ui->armor_widget->setValues(system, &current_class->armor_set, RPGSystem::ARMORS);

    this->ui->state_widget->setStates(system, &current_class->state_ranks);
    this->ui->element_widget->setElements(system, &current_class->element_ranks);



    this->ui->table_skills->clearContents();
    this->ui->table_skills->setRowCount(0);
    for (int i = 0; i < current_class->learnings_level.length(); i++)
    {
        int level = current_class->learnings_level.at(i);
        int skill_id = current_class->learnings_skill_id.at(i);
        QString skill_name = system->datasource.get_obj_name_by_id(skill_id, RPGSystem::SKILLS, true, 3, false);

        QTableWidgetItem *item;

        this->ui->table_skills->setRowCount(this->ui->table_skills->rowCount()+1);
        this->ui->table_skills->setItem(i,0, item = new QTableWidgetItem("Lv. " + QString::number(level)));
        this->set_readonly(item);
        this->ui->table_skills->setItem(i,1, item = new QTableWidgetItem(skill_name));
        this->set_readonly(item);
        this->ui->table_skills->setItem(i,2, new QTableWidgetItem(QString::number(level)));
        this->ui->table_skills->setItem(i,3, new QTableWidgetItem(QString::number(skill_id)));
    }

    this->ui->table_skills->resizeColumnsToContents();

    //qDebug() << current_class->to_hash();
    */
}

void EditClasses::save()
{
    /*
    int n = this->current;
    if (this->system->classes_list.length() <= n) return;
    RPGClass *current_class = this->system->classes_list.at(n);

    current_class->name = this->ui->line_name->text();
    current_class->position = this->ui->combo_pos->currentIndex();

    //this->ui->weapon_widget->getValues(&current_class->weapon_set);
    //this->ui->armor_widget->getValues(&current_class->armor_set);

    this->ui->state_widget->getValues(&current_class->state_ranks);
    this->ui->element_widget->getValues(&current_class->element_ranks);


    current_class->learnings_level.clear();
    current_class->learnings_skill_id.clear();

    for (int i = 0; i < this->ui->table_skills->rowCount(); i++)
    {
        int level = this->ui->table_skills->item(i,2)->text().toInt();
        int skill_id = this->ui->table_skills->item(i,3)->text().toInt();

        current_class->learnings_level.append(level);
        current_class->learnings_skill_id.append(skill_id);
    }


    //qDebug() << current_class->to_hash();
    */
}

void EditClasses::set_skill_from_dialog(int row, int level, int skill)
{
    /*
    QString skill_name = system->datasource.get_obj_name_by_id(skill, RPGSystem::SKILLS, true, 3, false);

    if (row == -1)
    {
        int row = this->ui->table_skills->rowCount();
        this->ui->table_skills->setRowCount(row+1);


        QTableWidgetItem *item;
        this->ui->table_skills->setItem(row,0, item = new QTableWidgetItem("Lv. " + QString::number(level)));
        this->set_readonly(item);
        this->ui->table_skills->setItem(row,1, item = new QTableWidgetItem(skill_name));
        this->set_readonly(item);
        this->ui->table_skills->setItem(row,2, new QTableWidgetItem(QString::number(level)));
        this->ui->table_skills->setItem(row,3, new QTableWidgetItem(QString::number(skill)));

    }
    else
    {
        this->ui->table_skills->item(row,0)->setText("Lv. " + QString::number(level));
        this->ui->table_skills->item(row,1)->setText(skill_name);
        this->ui->table_skills->item(row,2)->setText(QString::number(level));
        this->ui->table_skills->item(row,3)->setText(QString::number(skill));
    }
    */
}

void EditClasses::on_table_skills_itemDoubleClicked(QTableWidgetItem *item)
{
    /*
    int level = this->ui->table_skills->item(item->row(), 2)->text().toInt();
    int skill = this->ui->table_skills->item(item->row(), 3)->text().toInt();

    SkillLearning *dialog = new SkillLearning(system,item->row(),level,skill, 0);
    connect(dialog, SIGNAL(ok_clicked(int,int,int)), this, SLOT(set_skill_from_dialog(int,int,int)));
    dialog->show();
    */
}


void EditClasses::on_button_skill_add_clicked()
{
    /*
    SkillLearning *dialog = new SkillLearning(system, -1 , 1, 1, 0);
    connect(dialog, SIGNAL(ok_clicked(int,int,int)), this, SLOT(set_skill_from_dialog(int,int,int)));
    dialog->show();
    */
}

void EditClasses::on_button_skill_del_clicked()
{
    //this->ui->table_skills->removeRow(this->ui->table_skills->currentRow());
}
