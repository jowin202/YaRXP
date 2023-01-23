#include "screenshakedialog.h"
#include "ui_screenshakedialog.h"

ScreenShakeDialog::ScreenShakeDialog(QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenShakeDialog)
{
    ui->setupUi(this);
    this->ui->spin_power->setValue(parameters.at(0).toInt());
    this->ui->spin_speed->setValue(parameters.at(1).toInt());
    this->ui->spin_time->setValue(parameters.at(2).toInt());
}

ScreenShakeDialog::~ScreenShakeDialog()
{
    delete ui;
}

void ScreenShakeDialog::on_button_cancel_clicked()
{
    this->close();
}


void ScreenShakeDialog::on_button_ok_clicked()
{
    QJsonArray p;
    p.append(this->ui->spin_power->value());
    p.append(this->ui->spin_speed->value());
    p.append(this->ui->spin_time->value());

    emit ok_clicked(p);
    this->close();
}

