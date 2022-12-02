#include "dataeditor.h"
#include "ui_dataeditor.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

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

void DataEditor::setSystem(RPGSystem *system, RPGDB *db)
{
    this->db = db;
    this->ec = db->prepare_data_editor();

    this->system = system;
    this->ui->page_actors->setEC(ec);
    this->ui->page_classes->setEC(ec);
    this->ui->page_skills->setEC(ec);

    this->ui->page_items->setEC(ec);

    this->ui->page_weapons->setEC(ec);

    this->ui->page_armors->setEC(ec);

    this->ui->page_enemies->setEC(ec);
    this->ui->page_troops->setSystem(system);
    this->ui->page_states->setEC(ec);
    this->ui->page_animations->setSystem(system);
    this->ui->page_tilesets->setEC(ec);
    this->ui->page_commonevents->setSystem(system);
}



void DataEditor::on_object_list_currentRowChanged(int currentRow)
{
    if (currentRow < 0) return;

    this->ec->set_current_object(this->ui->central_menu->currentRow(), currentRow+1);


    if (this->ui->central_menu->currentRow() == TROOPS)
    {
        this->ui->page_troops->set_troop(currentRow);
    }
    else if (this->ui->central_menu->currentRow() == ANIMATIONS)
    {
        this->ui->page_animations->set_animation(currentRow);
    }
    else if (this->ui->central_menu->currentRow() == TILESETS)
    {
        this->ui->page_tilesets->set_tileset(currentRow);
    }
    else if (this->ui->central_menu->currentRow() == COMMONEVENTS)
    {
        this->ui->page_commonevents->set_commonevent(currentRow);
    }
}

void DataEditor::on_button_cancel_clicked()
{
    this->close();
}

void DataEditor::on_central_menu_currentRowChanged(int currentRow)
{
    if (system == 0) return;
    if (currentRow >= 0)
        this->ui->label->setText(this->ui->central_menu->item(currentRow)->text());

    if (currentRow >= 0 && currentRow <= 13)
        this->ui->stackedWidget->setCurrentIndex(currentRow);

    system->datasource.fill_list(this->ui->object_list, this->ui->central_menu->currentRow(), true, 3);

    if (this->ui->object_list->count() > 0)
        this->ui->object_list->setCurrentRow(0);
}


void DataEditor::on_button_apply_clicked()
{

}
