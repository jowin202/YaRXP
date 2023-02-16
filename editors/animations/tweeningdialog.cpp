#include "tweeningdialog.h"
#include "ui_tweeningdialog.h"

TweeningDialog::TweeningDialog(int max_frames, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TweeningDialog)
{
    ui->setupUi(this);
    this->ui->spin_frames_from->setMaximum(max_frames);
    this->ui->spin_frames_to->setMaximum(max_frames);
    this->ui->spin_frames_to->setValue(max_frames);
}

TweeningDialog::~TweeningDialog()
{
    delete ui;
}

void TweeningDialog::on_button_ok_clicked()
{

}


void TweeningDialog::on_button_cancel_clicked()
{
    this->close();
}

