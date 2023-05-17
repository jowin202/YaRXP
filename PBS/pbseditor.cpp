#include "pbseditor.h"
#include "ui_pbseditor.h"

PBSEditor::PBSEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PBSEditor)
{
    ui->setupUi(this);
}

PBSEditor::~PBSEditor()
{
    delete ui;
}
