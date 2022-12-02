#include "editsystem.h"
#include "ui_editsystem.h"

EditSystem::EditSystem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditSystem)
{
    ui->setupUi(this);
}

EditSystem::~EditSystem()
{
    delete ui;
}
