#include "objectcombodialog.h"
#include "ui_objectcombodialog.h"


ObjectComboDialog::ObjectComboDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ObjectComboDialog)
{
    ui->setupUi(this);
}

ObjectComboDialog::~ObjectComboDialog()
{
    delete ui;
}

void ObjectComboDialog::setText(QString text)
{
    this->ui->text->setText(text);
}


void ObjectComboDialog::on_button_ok_clicked()
{
    emit this->ok_clicked(this->ui->comboBox->currentData().toInt());
    this->close();
}

void ObjectComboDialog::on_button_cancel_clicked()
{
    this->close();
}
