#include "plusminuslist.h"
#include "ui_plusminuslist.h"

#include "plusminusbox.h"


PlusMinusList::PlusMinusList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlusMinusList)
{
    ui->setupUi(this);
    this->ui->verticalLayout->setSpacing(0);
    this->ui->verticalLayout->setAlignment(Qt::AlignTop);
}

PlusMinusList::~PlusMinusList()
{
    delete ui;
}

void PlusMinusList::add_box(QString name, int val)
{
    PlusMinusBox *box = new PlusMinusBox(name);
    if (val == 1)
        box->setPlus();
    else if (val == 2)
        box->setMinus();
    this->ui->verticalLayout->addWidget(box);
    connect(box, &PlusMinusBox::value_changed, this, [=]() { emit values_changed(); });

}

QJsonArray PlusMinusList::get_plus()
{
    QJsonArray array;
    for (int i = 0; i < this->ui->verticalLayout->count(); i++)
    {
        int v = ((PlusMinusBox*)this->ui->verticalLayout->itemAt(i)->widget())->value();
        if (v==1) array.append(i+1);
    }
    return array;
}

QJsonArray PlusMinusList::get_minus()
{
    QJsonArray array;
    for (int i = 0; i < this->ui->verticalLayout->count(); i++)
    {
        int v = ((PlusMinusBox*)this->ui->verticalLayout->itemAt(i)->widget())->value();
        if (v==2) array.append(i+1);
    }
    return array;
}

void PlusMinusList::clear()
{
    QLayoutItem *item;
    while ( (item = this->ui->verticalLayout->takeAt(0)) != 0)
        delete item->widget();
}

