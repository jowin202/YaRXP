#include "dealdamagedialog.h"
#include "ui_dealdamagedialog.h"

#include "RXIO2/rpgdb.h"

DealDamageDialog::DealDamageDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DealDamageDialog)
{
    ui->setupUi(this);

    this->ui->variable_widget->setVariableWidget(db);


    if (parameters.at(0).toInt() == 0)
    {
        this->ui->radio_enemy->setChecked(true);
        this->ui->combo_enemy->setCurrentIndex(parameters.at(1).toInt()+1);
    }
    else
    {
        this->ui->radio_actor->setChecked(true);
        this->ui->combo_actor->setCurrentIndex(parameters.at(1).toInt()+1);
    }


    if (parameters.at(2).toInt() == 0)
    {
        this->ui->radio_constant->setChecked(true);
        this->ui->spin_constant->setValue(parameters.at(3).toInt());
    }
    else
    {
        this->ui->radio_variable->setChecked(true);
        this->ui->variable_widget->setValue(parameters.at(3).toInt());
    }
}

DealDamageDialog::~DealDamageDialog()
{
    delete ui;
}

void DealDamageDialog::on_button_ok_clicked()
{
    QJsonArray p;
    if (this->ui->radio_enemy->isChecked())
    {
        p.append(0);
        p.append(this->ui->combo_enemy->currentIndex()-1);
    }
    else if (this->ui->radio_actor->isChecked())
    {
        p.append(1);
        p.append(this->ui->combo_actor->currentIndex()-1);
    }

    if (this->ui->radio_constant->isChecked())
    {
        p.append(0);
        p.append(this->ui->spin_constant->value());
    }
    else if (this->ui->radio_variable->isChecked())
    {
        p.append(1);
        p.append(this->ui->variable_widget->getValue());
    }

    this->close();
    emit ok_clicked(p);
}


void DealDamageDialog::on_button_cancel_clicked()
{
    this->close();
}

