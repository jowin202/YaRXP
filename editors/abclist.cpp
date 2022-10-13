#include "abclist.h"
#include "ui_abclist.h"

#include "RXIO/RXObjects/rpgsystem.h"
#include "abcbox.h"

ABCList::ABCList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ABCList)
{
    ui->setupUi(this);
    ui->verticalLayout->setSpacing(0);
    this->ui->verticalLayout->setAlignment(Qt::AlignTop);
}

ABCList::~ABCList()
{
    delete ui;
}

void ABCList::setStates(RPGSystem *system, QList<int> *ranks)
{
    this->clear();
    for (int i = 0; i < system->states_list.length(); i++)
    {
        ABCBox *combo_element = new ABCBox(ranks->at(6+2*i), system->states_list.at(i)->name);
        this->ui->verticalLayout->addWidget(combo_element);
    }
}

void ABCList::setElements(RPGSystem *system, QList<int> *ranks)
{
    this->clear();
    //start counting at 1
    for (int i = 1; i < system->elements.length(); i++)
    {
        ABCBox *combo_element = new ABCBox(ranks->at(4+2*i), system->elements.at(i));
        this->ui->verticalLayout->addWidget(combo_element);
    }
}

void ABCList::getValues(QList<int> *ranks)
{
    ranks->clear();
    ranks->append(this->ui->verticalLayout->count()+1);
    ranks->append(1);
    ranks->append(1);
    ranks->append(1);
    ranks->append(0);
    ranks->append(0);

    for (int i = 0; i < this->ui->verticalLayout->count(); i++)
    {
        int v = ((ABCBox*)this->ui->verticalLayout->itemAt(i)->widget())->getValue();
        ranks->append(v);
        ranks->append(0);
    }
}

void ABCList::clear()
{
    QLayoutItem *item;
    while ( (item = this->ui->verticalLayout->takeAt(0)) != 0)
        delete item->widget();
}
