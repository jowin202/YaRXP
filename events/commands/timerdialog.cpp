#include "timerdialog.h"
#include "ui_timerdialog.h"

TimerDialog::TimerDialog(QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimerDialog)
{
    ui->setupUi(this);

    if (parameters.at(0).toInt() == 0)
    {
        this->ui->radio_start->setChecked(true);
        this->ui->spin_min->setValue(parameters.at(1).toInt() / 60);
        this->ui->spin_sec->setValue(parameters.at(1).toInt() % 60);
    }
    else
    {
        this->ui->radio_stop->setChecked(true);
    }
}

TimerDialog::~TimerDialog()
{
    delete ui;
}

void TimerDialog::on_button_ok_clicked()
{
    QJsonArray p;

    if (this->ui->radio_start->isChecked())
    {
        p.append(0);
        p.append(this->ui->spin_min->value() * 60 + this->ui->spin_sec->value());
    }
    else
        p.append(1);

    this->close();
    emit ok_clicked(p);
}


void TimerDialog::on_button_cancel_clicked()
{
    this->close();
}

