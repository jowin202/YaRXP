#include "nameinputprocessingdialog.h"
#include "ui_nameinputprocessingdialog.h"

#include "RXIO2/rpgdb.h"

NameInputProcessingDialog::NameInputProcessingDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NameInputProcessingDialog)
{
    ui->setupUi(this);

    db->fill_combo(this->ui->comboBox, RPGDB::ACTORS, true, 3);
    this->ui->comboBox->setCurrentIndex(parameters.at(0).toInt()-1);
    this->ui->spinBox->setValue(parameters.at(1).toInt());
}

NameInputProcessingDialog::~NameInputProcessingDialog()
{
    delete ui;
}

void NameInputProcessingDialog::on_button_cancel_clicked()
{
    this->close();
}


void NameInputProcessingDialog::on_button_ok_clicked()
{
    QJsonArray p;
    p.append(this->ui->comboBox->currentIndex()+1);
    p.append(this->ui->spinBox->value());
    this->close();
    emit ok_clicked(p);
}

