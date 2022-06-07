#include "commoneventseditor.h"
#include "ui_commoneventseditor.h"

CommonEventsEditor::CommonEventsEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonEventsEditor)
{
    ui->setupUi(this);
}

CommonEventsEditor::~CommonEventsEditor()
{
    delete ui;
}
