#include "combospindialog.h"
#include "ui_combospindialog.h"

#include "RXIO2/rpgdb.h"

ComboSpinDialog::ComboSpinDialog(RPGDB *db, int code, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComboSpinDialog)
{
    ui->setupUi(this);

    if (code == 103)
    {
        this->setWindowTitle("Input Number");
        //no change in dialog because its standard

        for (int i = 1; i < db->get_variable_names().count(); i++)
            this->ui->comboBox->addItem(QString("%1: %2").arg(i,4,10,QChar('0')).arg(db->get_variable_name(i)));
        this->ui->comboBox->setCurrentIndex(parameters.at(0).toInt()-1);
        this->ui->spinBox->setValue(parameters.at(1).toInt());

    }
    else if (code == 303)
    {
        this->setWindowTitle("Name Input Processing");
        this->ui->label->setText("Actor:");
        this->ui->label_2->setText("Max characters:");
        this->ui->spinBox->setMaximum(16);
        db->fill_combo(this->ui->comboBox, RPGDB::ACTORS, true, 3);
        this->ui->comboBox->setCurrentIndex(parameters.at(0).toInt()-1);
        this->ui->spinBox->setValue(parameters.at(1).toInt());
    }

}

ComboSpinDialog::~ComboSpinDialog()
{
    delete ui;
}

void ComboSpinDialog::on_button_cancel_clicked()
{
    this->close();
}


void ComboSpinDialog::on_button_ok_clicked()
{
    //both cases do the same
    QJsonArray p;
    p.append(this->ui->comboBox->currentIndex()+1);
    p.append(this->ui->spinBox->value());
    emit ok_clicked(p);
    this->close();
}

