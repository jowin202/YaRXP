#include "variabledialog.h"
#include "ui_variabledialog.h"

VariableDialog::VariableDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VariableDialog)
{
    ui->setupUi(this);
}

VariableDialog::~VariableDialog()
{
    delete ui;
}
