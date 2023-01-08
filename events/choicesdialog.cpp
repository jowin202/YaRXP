#include "choicesdialog.h"
#include "ui_choicesdialog.h"

ChoicesDialog::ChoicesDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChoicesDialog)
{
    ui->setupUi(this);
}

ChoicesDialog::~ChoicesDialog()
{
    delete ui;
}
