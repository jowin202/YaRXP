#include "switchdialog.h"
#include "ui_switchdialog.h"

SwitchDialog::SwitchDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchDialog)
{
    ui->setupUi(this);
}

SwitchDialog::~SwitchDialog()
{
    delete ui;
}
