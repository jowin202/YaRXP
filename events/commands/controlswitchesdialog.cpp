#include "controlswitchesdialog.h"
#include "ui_controlswitchesdialog.h"

#include "RXIO2/rpgdb.h"

ControlSwitchesDialog::ControlSwitchesDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlSwitchesDialog)
{
    ui->setupUi(this);

    this->ui->widget_switch->setSwitchWidget(db);

    if (parameters.at(0).toInt() == parameters.at(1).toInt())
    {
        this->ui->radio_single->setChecked(true);
        this->ui->widget_switch->setValue(parameters.at(0).toInt());
    }
    else
    {
        this->ui->radio_batch->setChecked(true);
        this->ui->spin_from->setValue(parameters.at(0).toInt());
        this->ui->spin_to->setValue(parameters.at(1).toInt());
    }

    if (parameters.at(2).toInt() == 0)
        this->ui->radio_on->setChecked(true);
    else
        this->ui->radio_off->setChecked(true);
}

ControlSwitchesDialog::~ControlSwitchesDialog()
{
    delete ui;
}

void ControlSwitchesDialog::on_button_ok_clicked()
{
    QJsonArray p;
    if (this->ui->radio_single->isChecked())
    {
        p.append(this->ui->widget_switch->getValue());
        p.append(this->ui->widget_switch->getValue());
    }
    else if (this->ui->radio_batch->isChecked())
    {
        int a = this->ui->spin_from->value();
        int b = this->ui->spin_to->value();
        if (a < b)
        {
            p.append(a);
            p.append(b);
        }
        else
        {
            p.append(b);
            p.append(a);
        }
    }

    if (this->ui->radio_on->isChecked())
        p.append(0);
    else
        p.append(1);

    this->close();
    emit ok_clicked(p);
}


void ControlSwitchesDialog::on_button_cancel_clicked()
{
    this->close();
}

