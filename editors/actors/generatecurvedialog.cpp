#include "generatecurvedialog.h"
#include "ui_generatecurvedialog.h"

#include <QtMath>


GenerateCurveDialog::GenerateCurveDialog(int *values, int max_val, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GenerateCurveDialog)
{
    ui->setupUi(this);
    this->values = values;

    this->ui->spin_max->setMaximum(max_val);
    this->ui->spin_min->setMaximum(max_val);
}

GenerateCurveDialog::~GenerateCurveDialog()
{
    delete ui;
}

void GenerateCurveDialog::on_button_ok_clicked()
{
    int min = this->ui->spin_min->value();
    int max = this->ui->spin_max->value();
    qreal rate = this->ui->slider_rate->value()/10.0;

    for (int i = 1; i <= 99; i++)
        this->values[i] = min + qFloor((1 - rate) * qCeil((max-min) * (i-1)/98.0) + rate * qCeil((max-min) * qPow((i-1)/98.0,2)));

    emit ok_clicked();
    this->close();
}

void GenerateCurveDialog::on_button_cancel_clicked()
{
    this->close();
}
