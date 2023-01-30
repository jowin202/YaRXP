#include "forceactiondialog.h"
#include "ui_forceactiondialog.h"

#include "RXIO2/rpgdb.h"

ForceActionDialog::ForceActionDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForceActionDialog)
{
    ui->setupUi(this);
    db->fill_combo(this->ui->combo_skill, RPGDB::SKILLS, true, 3);

    if (parameters.at(0).toInt() == 0)
    {
        this->ui->radio_enemy->setChecked(true);
        this->ui->combo_enemy->setCurrentIndex(parameters.at(1).toInt());
    }
    else
    {
        this->ui->radio_actor->setChecked(true);
        this->ui->combo_actor->setCurrentIndex(parameters.at(1).toInt());
    }

    if (parameters.at(2).toInt() == 0)
    {
        this->ui->radio_basic->setChecked(true);
        this->ui->combo_basic->setCurrentIndex(parameters.at(3).toInt());
    }
    else
    {
        this->ui->radio_skill->setChecked(true);
        this->ui->combo_skill->setCurrentIndex(parameters.at(3).toInt()-1);
    }

    this->ui->combo_target->setCurrentIndex(parameters.at(4).toInt()+2);

    if (parameters.at(5).toInt() == 0)
        this->ui->radio_normal_sequence->setChecked(true);
    else
        this->ui->radio_execute_now->setChecked(true);
}

ForceActionDialog::~ForceActionDialog()
{
    delete ui;
}

void ForceActionDialog::on_button_cancel_clicked()
{
    this->close();
}


void ForceActionDialog::on_button_ok_clicked()
{
    QJsonArray p;

    if (this->ui->radio_enemy->isChecked())
    {
        p.append(0);
        p.append(this->ui->combo_enemy->currentIndex());
    }
    else if (this->ui->radio_actor->isChecked())
    {
        p.append(1);
        p.append(this->ui->combo_actor->currentIndex());
    }

    if (this->ui->radio_basic->isChecked())
    {
        p.append(0);
        p.append(this->ui->combo_basic->currentIndex());
    }
    else
    {
        p.append(1);
        p.append(this->ui->combo_skill->currentIndex()+1);
    }

    p.append(this->ui->combo_target->currentIndex()-2);

    if (this->ui->radio_normal_sequence->isChecked())
        p.append(0);
    else if (this->ui->radio_execute_now->isChecked())
        p.append(1);

    this->close();
    emit ok_clicked(p);
}

