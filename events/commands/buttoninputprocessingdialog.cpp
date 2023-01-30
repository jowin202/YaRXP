#include "buttoninputprocessingdialog.h"
#include "ui_buttoninputprocessingdialog.h"

ButtonInputProcessingDialog::ButtonInputProcessingDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ButtonInputProcessingDialog)
{
    ui->setupUi(this);
    this->ui->widget_variable->setVariableWidget(db);
    this->ui->widget_variable->setValue(parameters.at(0).toInt());
}

ButtonInputProcessingDialog::~ButtonInputProcessingDialog()
{
    delete ui;
}

void ButtonInputProcessingDialog::on_button_ok_clicked()
{
    this->close();
    QJsonArray p;
    p.append(this->ui->widget_variable->getValue());
    emit ok_clicked(p);
}


void ButtonInputProcessingDialog::on_button_cancel_clicked()
{
    this->close();
}

