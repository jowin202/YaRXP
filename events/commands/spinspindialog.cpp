#include "spinspindialog.h"
#include "ui_spinspindialog.h"

SpinSpinDialog::SpinSpinDialog(int code, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpinSpinDialog)
{
    ui->setupUi(this);

    if (code == 206)
    {
        this->setWindowTitle("Change Fog Opacity");
        this->ui->label->setText("Opacity:");
        this->ui->label_2->setText("Time (Frames):");

        this->ui->spinBox->setMinimum(0);
        this->ui->spinBox->setMaximum(255);
        this->ui->spinBox_2->setMinimum(0);
        this->ui->spinBox_2->setMaximum(200);

        this->ui->spinBox->setValue(parameters.at(0).toInt());
        this->ui->spinBox_2->setValue(parameters.at(1).toInt());
    }
    else //code == 233
    {
        this->setWindowTitle("Rotate Picture");
        this->ui->label->setText("Number:");
        this->ui->label_2->setText("Speed:");

        this->ui->spinBox->setMinimum(1);
        this->ui->spinBox->setMaximum(50);
        this->ui->spinBox_2->setMinimum(-90);
        this->ui->spinBox_2->setMaximum(90);

        this->ui->spinBox->setValue(parameters.at(0).toInt());
        this->ui->spinBox_2->setValue(parameters.at(1).toInt());
    }
}

SpinSpinDialog::~SpinSpinDialog()
{
    delete ui;
}

void SpinSpinDialog::on_button_cancel_clicked()
{
    this->close();
}


void SpinSpinDialog::on_button_ok_clicked()
{
    QJsonArray p;
    p.append(this->ui->spinBox->value());
    p.append(this->ui->spinBox_2->value());
    emit ok_clicked(p);
    this->close();
}

