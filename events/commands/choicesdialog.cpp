#include "choicesdialog.h"
#include "ui_choicesdialog.h"

ChoicesDialog::ChoicesDialog(QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChoicesDialog)
{
    ui->setupUi(this);

    if (parameters.at(0).toArray().count() >= 1)
        this->ui->lineEdit->setText(parameters.at(0).toArray().at(0).toString());
    if (parameters.at(0).toArray().count() >= 2)
        this->ui->lineEdit_2->setText(parameters.at(0).toArray().at(1).toString());
    if (parameters.at(0).toArray().count() >= 3)
        this->ui->lineEdit_3->setText(parameters.at(0).toArray().at(2).toString());
    if (parameters.at(0).toArray().count() >= 4)
        this->ui->lineEdit_4->setText(parameters.at(0).toArray().at(3).toString());

    if (parameters.at(1).toInt() == 0)
        this->ui->radioButton->setChecked(true);
    else if (parameters.at(1).toInt() == 1)
        this->ui->radioButton_2->setChecked(true);
    else if (parameters.at(1).toInt() == 2)
        this->ui->radioButton_3->setChecked(true);
    else if (parameters.at(1).toInt() == 3)
        this->ui->radioButton_4->setChecked(true);
    else if (parameters.at(1).toInt() == 4)
        this->ui->radioButton_5->setChecked(true);
    else if (parameters.at(1).toInt() == 5)
        this->ui->radioButton_6->setChecked(true);
}

ChoicesDialog::~ChoicesDialog()
{
    delete ui;
}

void ChoicesDialog::on_button_ok_clicked()
{
    QJsonArray p;
    QJsonArray c;

    if (this->ui->lineEdit_4->text() != "")
    {
        c.append(this->ui->lineEdit->text());
        c.append(this->ui->lineEdit_2->text());
        c.append(this->ui->lineEdit_3->text());
        c.append(this->ui->lineEdit_4->text());
    }
    else if (this->ui->lineEdit_3->text() != "")
    {
        c.append(this->ui->lineEdit->text());
        c.append(this->ui->lineEdit_2->text());
        c.append(this->ui->lineEdit_3->text());
    }
    else if (this->ui->lineEdit_2->text() != "")
    {
        c.append(this->ui->lineEdit->text());
        c.append(this->ui->lineEdit_2->text());
    }
    else if (this->ui->lineEdit->text() != "")
    {
        c.append(this->ui->lineEdit->text());
    }
    else
        c.append("");
    p.append(c);

    if (this->ui->radioButton->isChecked())
        p.append(0);
    else if (this->ui->radioButton_2->isChecked())
        p.append(1);
    else if (this->ui->radioButton_3->isChecked())
        p.append(2);
    else if (this->ui->radioButton_4->isChecked())
        p.append(3);
    else if (this->ui->radioButton_5->isChecked())
        p.append(4);
    else if (this->ui->radioButton_6->isChecked())
        p.append(5);

    this->close();
    emit ok_clicked(p);
}


void ChoicesDialog::on_button_cancel_clicked()
{
    this->close();
}

