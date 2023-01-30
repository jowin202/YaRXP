#include "changetextoptionsdialog.h"
#include "ui_changetextoptionsdialog.h"

ChangeTextOptionsDialog::ChangeTextOptionsDialog(QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeTextOptionsDialog)
{
    ui->setupUi(this);

    if (parameters.at(0).toInt() == 0) this->ui->radio_pos_top->setChecked(true);
    else if (parameters.at(0).toInt() == 1) this->ui->radio_pos_middle->setChecked(true);
    else if (parameters.at(0).toInt() == 2) this->ui->radio_pos_bottom->setChecked(true);

    if (parameters.at(1).toInt() == 0) this->ui->radio_win_show->setChecked(true);
    else if (parameters.at(1).toInt() == 1) this->ui->radio_win_hide->setChecked(true);
}

ChangeTextOptionsDialog::~ChangeTextOptionsDialog()
{
    delete ui;
}


void ChangeTextOptionsDialog::on_button_ok_clicked()
{
    QJsonArray parameters;
    if (this->ui->radio_pos_top->isChecked()) parameters.append(0);
    else if (this->ui->radio_pos_middle->isChecked()) parameters.append(1);
    else if (this->ui->radio_pos_bottom->isChecked()) parameters.append(2);


    if (this->ui->radio_win_show->isChecked()) parameters.append(0);
    else if (this->ui->radio_win_hide->isChecked()) parameters.append(1);

    this->close();
    emit ok_clicked(parameters);
}


void ChangeTextOptionsDialog::on_button_cancel_clicked()
{
    this->close();
}

