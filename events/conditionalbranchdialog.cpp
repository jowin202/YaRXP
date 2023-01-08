#include "conditionalbranchdialog.h"
#include "ui_conditionalbranchdialog.h"

ConditionalBranchDialog::ConditionalBranchDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConditionalBranchDialog)
{
    ui->setupUi(this);
}

ConditionalBranchDialog::~ConditionalBranchDialog()
{
    delete ui;
}
