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

void DataEditor::setSystem(RPGSystem *system)
{
    this->system = system;
    this->ui->page_actors->setSystem(system);
    this->ui->page_classes->setSystem(system);
}


void DataEditor::on_object_list_currentRowChanged(int currentRow)
{
    if (currentRow < 0) return;

    if (this->ui->central_menu->currentRow() == ACTORS)
    {
        this->ui->page_actors->set_actor(currentRow);
    }
    else if (this->ui->central_menu->currentRow() == CLASSES)
    {
        this->ui->page_classes->set_class(currentRow);
    }
}

void DataEditor::on_button_cancel_clicked()
{
    this->close();
}

void DataEditor::on_central_menu_currentRowChanged(int currentRow)
{
    if (system == 0) return;
    if (currentRow > 0)
        this->ui->label->setText(this->ui->central_menu->item(currentRow)->text());

    if (currentRow >= 0 && currentRow <= 13)
        this->ui->stackedWidget->setCurrentIndex(currentRow);

    system->datasource.fill_list(this->ui->object_list, this->ui->central_menu->currentRow(), true, 3);

    if (this->ui->object_list->count() > 0)
        this->ui->object_list->setCurrentRow(0);
}

