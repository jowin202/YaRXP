#include "dataeditor.h"
#include "ui_dataeditor.h"

DataEditor::DataEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataEditor)
{
    ui->setupUi(this);
}

DataEditor::~DataEditor()
{
    delete ui;
}

void DataEditor::set_widget(int widget)
{
    this->ui->central_menu->setCurrentRow(widget);
}

void DataEditor::on_central_menu_itemSelectionChanged()
{

    if (this->ui->central_menu->selectedItems().length() > 0)
        this->ui->label->setText(this->ui->central_menu->selectedItems().first()->text());

    if (this->ui->central_menu->currentRow() >= 0 && this->ui->central_menu->currentRow() <= 13)
        this->ui->stackedWidget->setCurrentIndex(this->ui->central_menu->currentRow());

}
