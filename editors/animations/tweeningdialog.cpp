#include "tweeningdialog.h"
#include "ui_tweeningdialog.h"

TweeningDialog::TweeningDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TweeningDialog)
{
    ui->setupUi(this);
}

TweeningDialog::~TweeningDialog()
{
    delete ui;
}
