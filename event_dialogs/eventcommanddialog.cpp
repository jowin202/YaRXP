#include "eventcommanddialog.h"
#include "ui_eventcommanddialog.h"

EventCommandDialog::EventCommandDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventCommandDialog)
{
    ui->setupUi(this);
}

EventCommandDialog::~EventCommandDialog()
{
    delete ui;
}
