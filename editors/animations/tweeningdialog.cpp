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
    this->close();
    emit ok_clicked(this->ui->spin_frames_from->value(),this->ui->spin_frames_to->value(),
                    this->ui->spin_cells_from->value(),this->ui->spin_cells_to->value(),
                    this->ui->check_pattern->isChecked(), this->ui->check_position->isChecked(), this->ui->check_opacity->isChecked());
}


void TweeningDialog::on_button_cancel_clicked()
{
    this->close();
}

