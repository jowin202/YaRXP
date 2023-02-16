#include "cellbatchdialog.h"
#include "ui_cellbatchdialog.h"

CellBatchDialog::CellBatchDialog(int max_frames, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellBatchDialog)
{
    ui->setupUi(this);
    this->ui->spin_frames_from->setMaximum(max_frames);
    this->ui->spin_frames_to->setMaximum(max_frames);
}

CellBatchDialog::~CellBatchDialog()
{
    delete ui;
}

void CellBatchDialog::on_button_ok_clicked()
{

}


void CellBatchDialog::on_button_cancel_clicked()
{
    this->close();
}

