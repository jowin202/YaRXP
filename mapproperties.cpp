#include "mapproperties.h"
#include "ui_mapproperties.h"

MapProperties::MapProperties(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapProperties)
{
    ui->setupUi(this);
}

MapProperties::~MapProperties()
{
    delete ui;
}
