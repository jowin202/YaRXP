#include "changepartymemberdialog.h"
#include "ui_changepartymemberdialog.h"

#include "RXIO2/rpgdb.h"

ChangePartyMemberDialog::ChangePartyMemberDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangePartyMemberDialog)
{
    ui->setupUi(this);
    db->fill_combo(this->ui->comboBox, RPGDB::ACTORS, true, 3);
    this->ui->comboBox->setCurrentIndex(parameters.at(0).toInt()-1);
    if (parameters.at(1).toInt() == 0)
        this->ui->radio_add->setChecked(true);
    if (parameters.at(1).toInt() == 1)
        this->ui->radio_remove->setChecked(true);

    this->ui->checkBox->setChecked(parameters.at(2).toInt() == 1);
}

ChangePartyMemberDialog::~ChangePartyMemberDialog()
{
    delete ui;
}

void ChangePartyMemberDialog::on_button_cancel_clicked()
{
    this->close();
}


void ChangePartyMemberDialog::on_button_ok_clicked()
{
    QJsonArray p;
    p.append(this->ui->comboBox->currentIndex()+1);
    if (this->ui->radio_add->isChecked())
        p.append(0);
    else
        p.append(1);

    if (this->ui->checkBox->isChecked())
        p.append(1);
    else
        p.append(0);

    this->close();
    emit ok_clicked(p);
}

