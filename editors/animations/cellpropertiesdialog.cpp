#include "cellpropertiesdialog.h"
#include "ui_cellpropertiesdialog.h"

CellPropertiesDialog::CellPropertiesDialog(QJsonArray values, int number, int max, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellPropertiesDialog)
{
    ui->setupUi(this);
    this->ui->spin_pattern->setValue(values.at(0*max+number).toInt()+1);
    this->ui->spin_x->setValue(values.at(1*max+number).toInt());
    this->ui->spin_y->setValue(values.at(2*max+number).toInt());
    this->ui->spin_zoom->setValue(values.at(3*max+number).toInt());
    this->ui->spin_angle->setValue(values.at(4*max+number).toInt());
    this->ui->combo_flip->setCurrentIndex(values.at(5*max+number).toInt());
    this->ui->spin_opacity->setValue(values.at(6*max+number).toInt());
    this->ui->combo_blending->setCurrentIndex(values.at(7*max+number).toInt());

    this->values = values;
    this->number = number;
    this->max = max;
}

CellPropertiesDialog::~CellPropertiesDialog()
{
    delete ui;
}

void CellPropertiesDialog::on_button_ok_clicked()
{
    values.removeAt(0*max+number);
    values.insert(0*max+number, this->ui->spin_pattern->value()-1);
    values.removeAt(1*max+number);
    values.insert(1*max+number, this->ui->spin_x->value());
    values.removeAt(2*max+number);
    values.insert(2*max+number, this->ui->spin_y->value());
    values.removeAt(3*max+number);
    values.insert(3*max+number, this->ui->spin_zoom->value());
    values.removeAt(4*max+number);
    values.insert(4*max+number, this->ui->spin_angle->value());
    values.removeAt(5*max+number);
    values.insert(5*max+number, this->ui->combo_flip->currentIndex());
    values.removeAt(6*max+number);
    values.insert(6*max+number, this->ui->spin_opacity->value());
    values.removeAt(7*max+number);
    values.insert(7*max+number, this->ui->combo_blending->currentIndex());

    this->close();
    emit ok_clicked(this->values);
}


void CellPropertiesDialog::on_button_cancel_clicked()
{
    this->close();
}

