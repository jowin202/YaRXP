#include "flashsedialog.h"
#include "ui_flashsedialog.h"

FlashSEDialog::FlashSEDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FlashSEDialog)
{
    ui->setupUi(this);
}

FlashSEDialog::~FlashSEDialog()
{
    delete ui;
}
