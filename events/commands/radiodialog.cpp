#include "radiodialog.h"
#include "ui_radiodialog.h"

RadioDialog::RadioDialog(QWidget *parent, int code, bool left) :
    QWidget(parent),
    ui(new Ui::RadioDialog)
{
    ui->setupUi(this);

    if (code == 134)
        this->setWindowTitle("Change Save Access");
    else if (code == 135)
        this->setWindowTitle("Change Menu Access");
    else if (code == 136)
        this->setWindowTitle("Change Encounter");
    if (code == 208)
    {
        this->ui->radio_left->setText("Transparent");
        this->ui->radio_right->setText("Normal");
        this->setWindowTitle("Change Transparent Flag");
        this->ui->groupBox->setTitle("Transparent Flag");
    }
    if (left)
    {
        this->ui->radio_left->setChecked(true);
        this->ui->radio_left->setFocus();
    }
    else
    {
        this->ui->radio_right->setChecked(true);
        this->ui->radio_right->setFocus();
    }
}

RadioDialog::~RadioDialog()
{
    delete ui;
}

void RadioDialog::on_button_cancel_clicked()
{
    this->close();
}


void RadioDialog::on_button_ok_clicked()
{
    QJsonArray array;
    if (this->ui->radio_left->isChecked())
        array.append(0);
    else
        array.append(1);
    emit ok_clicked(array);
    this->close();
}

