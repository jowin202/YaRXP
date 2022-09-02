#include "editclasses.h"
#include "ui_editclasses.h"

#include "classcheckbox.h"

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
        ClassCheckbox *check = new ClassCheckbox;
        check->setText(system->weapons_list.at(i)->name);
        check->setChecked(current_class->weapon_set.contains(system->weapons_list.at(i)->id));
        this->ui->weapon_layout->addWidget(check);
    }
    for (int i = 0; i < system->armors_list.length(); i++)
    {
        ClassCheckbox *check = new ClassCheckbox;
        check->setText(system->armors_list.at(i)->name);
        check->setChecked(current_class->armor_set.contains(system->armors_list.at(i)->id));
        this->ui->armor_layout->addWidget(check);
    }
}
