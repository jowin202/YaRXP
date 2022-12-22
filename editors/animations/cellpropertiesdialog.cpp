#include "cellpropertiesdialog.h"
#include "ui_cellpropertiesdialog.h"

CellPropertiesDialog::CellPropertiesDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CellPropertiesDialog)
{
    ui->setupUi(this);
}

CellPropertiesDialog::~CellPropertiesDialog()
{
    delete ui;
}

void CellPropertiesDialog::on_button_ok_clicked()
{

}


void CellPropertiesDialog::on_button_cancel_clicked()
{

}

