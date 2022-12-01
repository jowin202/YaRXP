#include "abcbox.h"
#include "ui_abcbox.h"

ABCBox::ABCBox(int value, QString text, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ABCBox)
{
    ui->setupUi(this);
    this->ui->label->setText(text);
    this->ui->comboBox->setCurrentIndex(value-1);
}

ABCBox::~ABCBox()
{
    delete ui;
}

int ABCBox::getValue()
{
    return this->ui->comboBox->currentIndex()+1; //TODO Check this
}

void ABCBox::on_comboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    emit value_changed();
}

