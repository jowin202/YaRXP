#include "checkboxlist.h"
#include "ui_checkboxlist.h"

#include <QCheckBox>
#include "RXIO/RXObjects/rpgsystem.h"


CheckBoxList::CheckBoxList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckBoxList)
{
    ui->setupUi(this);
    this->ui->verticalLayout->setSpacing(0);
    this->ui->verticalLayout->setAlignment(Qt::AlignTop);
}

CheckBoxList::~CheckBoxList()
{
    delete ui;
}

void CheckBoxList::setValues(RPGSystem *system, QList<int> *set, int type)
{
    this->clear();
    if (type == RPGSystem::WEAPONS)
    {
        for (int i = 0; i < system->weapons_list.length(); i++)
        {
            QCheckBox *check = new QCheckBox;
            check->setText(system->weapons_list.at(i)->name);
            check->setChecked(set->contains(system->weapons_list.at(i)->id));
            this->ui->verticalLayout->addWidget(check);
        }
    }
    else if (type == RPGSystem::ARMORS)
    {
        for (int i = 0; i < system->armors_list.length(); i++)
        {
            QCheckBox *check = new QCheckBox;
            check->setText(system->armors_list.at(i)->name);
            check->setChecked(set->contains(system->armors_list.at(i)->id));
            this->ui->verticalLayout->addWidget(check);
        }
    }
    else if (type == RPGSystem::STATES)
    {
        for (int i = 0; i < system->states_list.length(); i++)
        {
            QCheckBox *check = new QCheckBox;
            check->setText(system->states_list.at(i)->name);
            check->setChecked(set->contains(system->states_list.at(i)->id));
            this->ui->verticalLayout->addWidget(check);
        }
    }
    else //Elements
    {
        //Elements starts counting at 1
        for (int i = 1; i < system->elements.length(); i++)
        {
            QCheckBox *check = new QCheckBox;
            check->setText(system->elements.at(i));
            check->setChecked(set->contains(i));
            this->ui->verticalLayout->addWidget(check);
        }
    }
}

void CheckBoxList::clear()
{
    QLayoutItem *item;
     while ( (item = this->ui->verticalLayout->takeAt(0)) != 0)
         delete item->widget();
}
