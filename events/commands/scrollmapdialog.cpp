#include "scrollmapdialog.h"
#include "ui_scrollmapdialog.h"

ScrollMapDialog::ScrollMapDialog(QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScrollMapDialog)
{
    ui->setupUi(this);
    this->ui->combo_dir->setCurrentIndex(parameters.at(0).toInt()/2-1);
    this->ui->spin_dist->setValue(parameters.at(1).toInt());
    this->ui->combo_speed->setCurrentIndex(parameters.at(2).toInt()-1);
}

ScrollMapDialog::~ScrollMapDialog()
{
    delete ui;
}

void ScrollMapDialog::on_button_cancel_clicked()
{
    this->close();
}


void ScrollMapDialog::on_button_ok_clicked()
{
    QJsonArray p;
    p.append((this->ui->combo_dir->currentIndex()+1)*2);
    p.append(this->ui->spin_dist->value());
    p.append(this->ui->combo_speed->currentIndex()+1);
    emit ok_clicked(p);
    this->close();
}

