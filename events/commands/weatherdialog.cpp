#include "weatherdialog.h"
#include "ui_weatherdialog.h"

WeatherDialog::WeatherDialog(int code, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WeatherDialog)
{
    ui->setupUi(this);

    Q_UNUSED(code);

    if (parameters.at(0).toInt() == 0)
        this->ui->radio_none->setChecked(true);
    else if (parameters.at(0).toInt() == 1)
        this->ui->radio_rain->setChecked(true);
    else if (parameters.at(0).toInt() == 2)
        this->ui->radio_storm->setChecked(true);
    else if (parameters.at(0).toInt() == 3)
        this->ui->radio_snow->setChecked(true);

    if(parameters.at(0).toInt() > 0)
    {
        this->ui->slider_power->setEnabled(true);
        this->ui->spin_power->setEnabled(true);
    }
    this->ui->spin_power->setValue(parameters.at(1).toInt());
    this->ui->spin_time->setValue(parameters.at(2).toInt());


}

WeatherDialog::~WeatherDialog()
{
    delete ui;
}

void WeatherDialog::on_button_cancel_clicked()
{
    this->close();
}


void WeatherDialog::on_button_ok_clicked()
{
    QJsonArray p;
    if (this->ui->radio_none->isChecked())
        p.append(0);
    else if (this->ui->radio_rain->isChecked())
        p.append(1);
    else if (this->ui->radio_storm->isChecked())
        p.append(2);
    else if (this->ui->radio_snow->isChecked())
        p.append(3);

    p.append(this->ui->spin_power->value());
    p.append(this->ui->spin_time->value());


    emit ok_clicked(p);
    this->close();
}

