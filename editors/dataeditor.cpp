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
    delete ec;
}

void DataEditor::set_widget(int widget)
{
    this->ui->tabWidget->setCurrentIndex(widget);
}

void DataEditor::setDB(RPGDB *db)
{
    this->db = db;
    this->ec = db->prepare_data_editor();

    this->ec->fill_list(this->ui->actor_list, RPGDB::ACTORS, true, 3, false);
    this->ui->actor_widget->setEC(ec);
    connect(this->ui->actor_list, &QListWidget::currentRowChanged, this, [=](int v){ ec->set_current_object(RPGDB::ACTORS, v+1); });
    this->ui->actor_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->classes_list, RPGDB::CLASSES, true, 3, false);
    this->ui->classes_widget->setEC(ec);
    connect(this->ui->classes_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::CLASSES, v+1); });
    this->ui->classes_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->skills_list, RPGDB::SKILLS, true, 3, false);
    this->ui->skills_widget->setEC(ec);
    connect(this->ui->skills_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::SKILLS, v+1); });
    this->ui->skills_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->items_list, RPGDB::ITEMS, true, 3, false);
    this->ui->items_widget->setEC(ec);
    connect(this->ui->items_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::ITEMS, v+1); });
    this->ui->items_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->weapons_list, RPGDB::WEAPONS, true, 3, false);
    this->ui->weapons_widget->setEC(ec);
    connect(this->ui->weapons_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::WEAPONS, v+1); });
    this->ui->weapons_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->armors_list, RPGDB::ARMORS, true, 3, false);
    this->ui->armors_widget->setEC(ec);
    connect(this->ui->armors_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::ARMORS, v+1); });
    this->ui->armors_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->enemies_list, RPGDB::ENEMIES, true, 3, false);
    this->ui->enemies_widget->setEC(ec);
    connect(this->ui->enemies_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::ENEMIES, v+1); });
    this->ui->enemies_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->troops_list, RPGDB::TROOPS, true, 3, false);
    this->ui->troops_widget->setEC(ec);
    connect(this->ui->troops_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::TROOPS, v+1); });
    this->ui->troops_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->states_list, RPGDB::STATES, true, 3, false);
    this->ui->states_widget->setEC(ec);
    connect(this->ui->states_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::STATES, v+1); });
    this->ui->states_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->animations_list, RPGDB::ANIMATIONS, true, 3, false);
    this->ui->animations_widget->setEC(ec);
    connect(this->ui->animations_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::ANIMATIONS, v+1); });
    this->ui->animations_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->tilesets_list, RPGDB::TILESETS, true, 3, false);
    this->ui->tilesets_widget->setEC(ec);
    connect(this->ui->tilesets_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::TILESETS, v+1); });
    this->ui->tilesets_list->setCurrentRow(0);

    this->ec->fill_list(this->ui->common_events_list, RPGDB::COMMONEVENTS, true, 3, false);
    this->ui->common_events_widget->setEC(ec);
    connect(this->ui->common_events_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::COMMONEVENTS, v+1); });
    this->ui->common_events_list->setCurrentRow(0);

    this->ui->system_widget->setEC(ec);

}



void DataEditor::on_button_cancel_clicked()
{
    this->close();
}


void DataEditor::on_button_apply_clicked()
{
    this->ec->save_edited_data();
}

void DataEditor::on_button_ok_clicked()
{
    this->on_button_apply_clicked();
    this->close();
}


void DataEditor::on_tabWidget_currentChanged(int index)
{
    this->ec->refresh(index);
}


void DataEditor::on_button_actor_max_clicked()
{
    this->set_maximum(RPGDB::ACTORS);
}


void DataEditor::on_button_classes_max_clicked()
{
    this->set_maximum(RPGDB::CLASSES);
}


void DataEditor::on_button_skills_max_clicked()
{
    this->set_maximum(RPGDB::SKILLS);
}


void DataEditor::on_button_items_max_clicked()
{
    this->set_maximum(RPGDB::ITEMS);
}


void DataEditor::on_button_weapons_max_clicked()
{
    this->set_maximum(RPGDB::WEAPONS);
}


void DataEditor::on_button_armors_max_clicked()
{
    this->set_maximum(RPGDB::ARMORS);
}


void DataEditor::on_button_enemies_max_clicked()
{
    this->set_maximum(RPGDB::ENEMIES);
}


void DataEditor::on_button_troops_max_clicked()
{
    this->set_maximum(RPGDB::TROOPS);
}


void DataEditor::on_button_states_max_clicked()
{
    this->set_maximum(RPGDB::STATES);
}


void DataEditor::on_button_animations_max_clicked()
{
    this->set_maximum(RPGDB::ANIMATIONS);
}


void DataEditor::on_button_tilesets_max_clicked()
{
    this->set_maximum(RPGDB::TILESETS);
}


void DataEditor::on_button_common_events_max_clicked()
{
    this->set_maximum(RPGDB::COMMONEVENTS);
}

void DataEditor::set_maximum(int obj_type)
{
    bool ok = false;
    int max = QInputDialog::getInt(this, "Select Maximum", "Set Maximum",this->ec->count_objects(obj_type), 0, 999,1, &ok);
    if (ok)
    {
        this->ec->set_max(obj_type, max);
        this->ec->refresh(obj_type);
    }
}

