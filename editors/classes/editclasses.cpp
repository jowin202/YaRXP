#include "editclasses.h"
#include "ui_editclasses.h"

EditClasses::EditClasses(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditClasses)
{
    ui->setupUi(this);
}

EditClasses::~EditClasses()
{
    delete ui;
}
