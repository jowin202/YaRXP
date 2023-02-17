#include "entireslidedialog.h"
#include "ui_entireslidedialog.h"

EntireSlideDialog::EntireSlideDialog(int max_frames, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntireSlideDialog)
{
    ui->setupUi(this);
    this->ui->spin_from->setMaximum(max_frames);
    this->ui->spin_to->setMaximum(max_frames);
    this->ui->spin_to->setValue(max_frames);
}

EntireSlideDialog::~EntireSlideDialog()
{
    delete ui;
}

void EntireSlideDialog::on_button_ok_clicked()
{
    this->close();
    emit ok_clicked(this->ui->spin_from->value(), this->ui->spin_to->value(),
                    this->ui->spin_x->value(), this->ui->spin_y->value());
}


void EntireSlideDialog::on_button_cancel_clicked()
{
    this->close();
}

