#include "systemeditor.h"
#include "ui_systemeditor.h"

SystemEditor::SystemEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemEditor)
{
    ui->setupUi(this);
}

SystemEditor::~SystemEditor()
{
    delete ui;
}
