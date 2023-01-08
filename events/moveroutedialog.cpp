#include "moveroutedialog.h"
#include "ui_moveroutedialog.h"

MoveRouteDialog::MoveRouteDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveRouteDialog)
{
    ui->setupUi(this);
}

MoveRouteDialog::~MoveRouteDialog()
{
    delete ui;
}
