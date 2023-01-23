#include "selfswitchdialog.h"
#include "ui_selfswitchdialog.h"

SelfSwitchDialog::SelfSwitchDialog(QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelfSwitchDialog)
{
    ui->setupUi(this);

    if (parameters.at(0).toString() == "A")
        this->ui->comboBox->setCurrentIndex(0);
    else if (parameters.at(0).toString() == "B")
        this->ui->comboBox->setCurrentIndex(1);
    else if (parameters.at(0).toString() == "C")
        this->ui->comboBox->setCurrentIndex(2);
    else //if (parameters.at(0).toString() == "D")
        this->ui->comboBox->setCurrentIndex(3);

    if (parameters.at(1).toInt() == 0)
        this->ui->radioButton->setChecked(true);
    else
        this->ui->radioButton_2->setChecked(true);

}

SelfSwitchDialog::~SelfSwitchDialog()
{
    delete ui;
}

void SelfSwitchDialog::on_button_cancel_clicked()
{
    this->close();
}


void SelfSwitchDialog::on_button_ok_clicked()
{
    QJsonArray p;

    if (this->ui->comboBox->currentIndex() == 0)
        p.append("A");
    else if (this->ui->comboBox->currentIndex() == 1)
        p.append("B");
    else if (this->ui->comboBox->currentIndex() == 2)
        p.append("C");
    else if (this->ui->comboBox->currentIndex() == 3)
        p.append("D");

    if (this->ui->radioButton->isChecked())
        p.append(0);
    else
        p.append(1);

    emit ok_clicked(p);
    this->close();
}

