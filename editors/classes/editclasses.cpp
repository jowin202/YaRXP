#include "editclasses.h"
#include "ui_editclasses.h"


#include "RXIO/RXObjects/rpgsystem.h"

EditClasses::EditClasses(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditClasses)
{
    ui->setupUi(this);

    this->ui->armor_layout->setAlignment(Qt::AlignTop);
    this->ui->weapon_layout->setAlignment(Qt::AlignTop);
}

EditClasses::~EditClasses()
{
    delete ui;
}

void EditClasses::set_class(int n)
{
    if (this->system->classes_list.length() <= n) return;
    RPGClass *current_class = system->classes_list.at(n);

    this->ui->line_name->setText(current_class->name);
    this->ui->combo_pos->setCurrentIndex(current_class->position);


    QLayoutItem *item;
    while ( (item = this->ui->weapon_layout->takeAt(0)) != 0)
        delete item->widget();
    while ( (item = this->ui->armor_layout->takeAt(0)) != 0)
        delete item->widget();

    for (int i = 0; i < system->weapons_list.length(); i++)
    {
        QCheckBox *check = new QCheckBox;
        check->setText(system->weapons_list.at(i)->name);
        check->setChecked(current_class->weapon_set.contains(system->weapons_list.at(i)->id));
        this->ui->weapon_layout->addWidget(check);
    }
    for (int i = 0; i < system->armors_list.length(); i++)
    {
        QCheckBox *check = new QCheckBox;
        check->setText(system->armors_list.at(i)->name);
        check->setChecked(current_class->armor_set.contains(system->armors_list.at(i)->id));
        this->ui->armor_layout->addWidget(check);
    }

    this->ui->table_skills->clearContents();
    this->ui->table_skills->setRowCount(0);
    for (int i = 0; i < current_class->learnings_level.length(); i++)
    {
        int level = current_class->learnings_level.at(i);
        int skill_id = current_class->learnings_skill_id.at(i);
        QString skill_name = system->datasource.get_obj_name_by_id(skill_id, RPGSystem::SKILLS, true, 3);

        this->ui->table_skills->setRowCount(this->ui->table_skills->rowCount()+1);
        this->ui->table_skills->setItem(i,0, new QTableWidgetItem(QString::number(level)));
        this->ui->table_skills->setItem(i,1, new QTableWidgetItem(skill_name));

    }

}
