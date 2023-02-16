#include "entireslidedialog.h"
#include "ui_entireslidedialog.h"

EntireSlideDialog::EntireSlideDialog(int max_frames, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntireSlideDialog)
{
    ui->setupUi(this);
    this->ui->spin_from->setMaximum(max_frames);
    this->ui->spin_to->setMaximum(max_frames);
}

EntireSlideDialog::~EntireSlideDialog()
{
    delete ui;
}

void EntireSlideDialog::on_button_ok_clicked()
{

}


void EntireSlideDialog::on_button_cancel_clicked()
{
    this->close();
}

