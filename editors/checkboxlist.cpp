#include "checkboxlist.h"
#include "ui_checkboxlist.h"

#include <QCheckBox>


//for typeid
#include <typeinfo>
#include <iostream>

#include "RXIO2/rpgeditorcontroller.h"


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

void CheckBoxList::add_checkbox(QString name, bool is_checked)
{
    QCheckBox *check = new QCheckBox(name);
    check->setChecked(is_checked);
    this->ui->verticalLayout->addWidget(check);
    connect(check, &QCheckBox::toggled, this, [=]() { emit values_changed(); });
}

QJsonArray CheckBoxList::get_result()
{
    QJsonArray list;
    for (int i = 0; i < this->ui->verticalLayout->count(); i++)
    {
        if (((QCheckBox*)this->ui->verticalLayout->itemAt(i)->widget())->isChecked())
            list.append(i+1); //should always be the id
    }
    return list;
}


void CheckBoxList::clear()
{
    QLayoutItem *item;
     while ( (item = this->ui->verticalLayout->takeAt(0)) != 0)
         delete item->widget();
}
