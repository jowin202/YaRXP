#include "plusminusbox.h"
#include "ui_plusminusbox.h"

PlusMinusBox::PlusMinusBox(QString text, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlusMinusBox)
{
    ui->setupUi(this);
    this->ui->label->setText(text);
    this->ui->toolButton->setText("");

    this->ui->toolButton->setMinimumWidth(40);
    this->ui->toolButton->setMinimumHeight(35);
}

PlusMinusBox::~PlusMinusBox()
{
    delete ui;
}

void PlusMinusBox::setPlus()
{
    this->ui->toolButton->setText("+");
}

void PlusMinusBox::setMinus()
{
    this->ui->toolButton->setText("-");
}

void PlusMinusBox::setFalse()
{
    this->ui->toolButton->setText("");
}

int PlusMinusBox::value()
{
    if (this->ui->toolButton->text() == "")
        return 0;
    else if (this->ui->toolButton->text() == "+")
        return 1;
    else //if (this->ui->toolButton->text() == "-")
        return 2;
}

void PlusMinusBox::on_toolButton_clicked()
{
    if (this->ui->toolButton->text() == "")
        this->ui->toolButton->setText("+");
    else if (this->ui->toolButton->text() == "+")
        this->ui->toolButton->setText("-");
    else if (this->ui->toolButton->text() == "-")
        this->ui->toolButton->setText("");


}
