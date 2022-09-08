#include "comboelement.h"
#include "ui_comboelement.h"

ComboElement::ComboElement(int value, QString text, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComboElement)
{
    ui->setupUi(this);
    this->ui->label_text->setText(text);
    this->ui->combo_value->setCurrentIndex(value-1);
}

ComboElement::~ComboElement()
{
    delete ui;
}


int ComboElement::getValue()
{
    return this->ui->combo_value->currentIndex();
}
