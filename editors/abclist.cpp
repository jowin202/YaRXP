#include "abclist.h"
#include "ui_abclist.h"

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

void ABCList::add_box(QString name, int value)
{
    ABCBox *box = new ABCBox(value, name);
    this->ui->verticalLayout->addWidget(box);
    connect(box, &ABCBox::value_changed, this, [=]() { emit values_changed(); });
}

QJsonArray ABCList::get_result()
{
    QJsonArray result;
    result.append(0); //first element is zero
    for (int i = 0; i < this->ui->verticalLayout->count(); i++)
    {
        result.append(QJsonValue( ((ABCBox*)this->ui->verticalLayout->itemAt(i)->widget())->getValue()));
    }

    return result;
}

void ABCList::clear()
{
    QLayoutItem *item;
    while ( (item = this->ui->verticalLayout->takeAt(0)) != 0)
        delete item->widget();
}
