#include "plusminuslist.h"
#include "ui_plusminuslist.h"

#include "plusminusbox.h"

#include "RXIO/RXObjects/rpgsystem.h"

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

void PlusMinusList::setStates(RPGSystem *system, QList<int> *plus_states, QList<int> *minus_states)
{

    QLayoutItem *item;
    while ( (item = this->ui->verticalLayout->takeAt(0)) != 0)
        delete item->widget();


    //state starts counting at 0
    for (int i = 0; i < system->states_list.length(); i++)
    {
        PlusMinusBox *checkbox = new PlusMinusBox(system->states_list.at(i)->name);
        if (plus_states->contains(i+1))
            checkbox->setPlus();
        else if (minus_states->contains(i+1))
            checkbox->setMinus();
        this->ui->verticalLayout->addWidget(checkbox);
    }
}
