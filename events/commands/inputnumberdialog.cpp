#include "inputnumberdialog.h"
#include "ui_inputnumberdialog.h"

InputNumberDialog::InputNumberDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputNumberDialog)
{
    ui->setupUi(this);
    this->ui->widget_variable->setVariableWidget(db);
    this->ui->widget_variable->setValue(parameters.at(0).toInt());
    this->ui->spinBox->setValue(parameters.at(1).toInt());
}

InputNumberDialog::~InputNumberDialog()
{
    delete ui;
}

void InputNumberDialog::on_button_cancel_clicked()
{
    this->close();
}


void InputNumberDialog::on_button_ok_clicked()
{
    QJsonArray p;
    p.append(this->ui->widget_variable->getValue());
    p.append(this->ui->spinBox->value());
    this->close();
    emit ok_clicked(p);
}

