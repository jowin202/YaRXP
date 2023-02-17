#include "cellbatchdialog.h"
#include "ui_cellbatchdialog.h"

CellBatchDialog::CellBatchDialog(int max_frames, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellBatchDialog)
{
    ui->setupUi(this);
    this->ui->spin_frames_from->setMaximum(max_frames);
    this->ui->spin_frames_to->setMaximum(max_frames);
    this->ui->spin_frames_to->setValue(max_frames);
}

CellBatchDialog::~CellBatchDialog()
{
    delete ui;
}

void CellBatchDialog::on_button_ok_clicked()
{
    this->close();
    emit ok_clicked(this->ui->spin_frames_from->value(), this->ui->spin_frames_to->value(),this->ui->spin_cells_from->value(), this->ui->spin_cells_to->value(),
                    this->ui->check_pattern->isChecked() ? this->ui->spin_pattern->value() : -1,
                    this->ui->check_x->isChecked() ? this->ui->spin_x->value() : -1,
                    this->ui->check_y->isChecked() ? this->ui->spin_y->value() : -1,
                    this->ui->check_zoom->isChecked() ? this->ui->spin_zoom->value() : -1,
                    this->ui->check_angle->isChecked() ? this->ui->spin_angle->value() : -1,
                    this->ui->check_flip->isChecked() ? this->ui->combo_flip->currentIndex() : -1,
                    this->ui->check_opacity->isChecked() ? this->ui->spin_opacity->value() : -1,
                    this->ui->check_blending->isChecked() ? this->ui->combo_blending->currentIndex() : -1);
}


void CellBatchDialog::on_button_cancel_clicked()
{
    this->close();
}

