#include "dataeditor.h"
#include "ui_dataeditor.h"

DataEditor::DataEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataEditor)
{
    ui->setupUi(this);
    this->ui->page_actors->set_system(system);
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
    this->ui->page_actors->set_system(system);
}

void DataEditor::on_central_menu_itemSelectionChanged()
{
    this->ui->object_list->clear();

    if (this->ui->central_menu->selectedItems().length() > 0)
        this->ui->label->setText(this->ui->central_menu->selectedItems().first()->text());

    if (this->ui->central_menu->currentRow() >= 0 && this->ui->central_menu->currentRow() <= 13)
        this->ui->stackedWidget->setCurrentIndex(this->ui->central_menu->currentRow());



    if (this->system != 0 && this->ui->central_menu->currentRow() == ACTORS)
        for (int i = 0; i < system->actor_list.length(); i++)
            this->ui->object_list->addItem(QString::number(i+1).rightJustified(3,'0') + ": " + system->actor_list.at(i)->name);
    else if (this->system != 0 && this->ui->central_menu->currentRow() == CLASSES)
        for (int i = 0; i < system->classes_list.length(); i++)
            this->ui->object_list->addItem(QString::number(i+1).rightJustified(3,'0') + ": " + system->classes_list.at(i)->name);
    else if (this->system != 0 && this->ui->central_menu->currentRow() == SKILLS)
        for (int i = 0; i < system->skills_list.length(); i++)
            this->ui->object_list->addItem(QString::number(i+1).rightJustified(3,'0') + ": " + system->skills_list.at(i)->name);
    else if (this->system != 0 && this->ui->central_menu->currentRow() == ITEMS)
        for (int i = 0; i < system->items_list.length(); i++)
            this->ui->object_list->addItem(QString::number(i+1).rightJustified(3,'0') + ": " + system->items_list.at(i)->name);


    if (this->ui->object_list->count() > 0)
    {
        this->ui->object_list->setCurrentRow(0);
    }

}

void DataEditor::on_object_list_currentRowChanged(int currentRow)
{
    if (currentRow < 0) return;

    if (this->ui->central_menu->currentRow() == ACTORS)
    {
        this->ui->page_actors->set_actor(currentRow);
    }
}

void DataEditor::on_button_cancel_clicked()
{
    this->close();
}
