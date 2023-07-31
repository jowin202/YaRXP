#include "dataeditor.h"
#include "ui_dataeditor.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/fileopener.h"
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


    this->ec->fill_list(this->ui->scripts_list, RPGDB::SCRIPTS, true, 3, false);
    this->ui->script_widget->setEC(ec);
    connect(this->ui->scripts_list, &QListWidget::currentRowChanged, ec, [=](int v){ ec->set_current_object(RPGDB::SCRIPTS, v); });
    this->ui->scripts_list->setCurrentRow(0);




    //ACTORS: CTRL+C  -- CTRL+V
    action_actors_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_actors_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_actors_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_actor", QJsonDocument(ec->get_object_by_id(RPGDB::ACTORS,this->ui->actor_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->actor_list->addAction(&action_actors_copy);

    action_actors_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_actors_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_actors_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_actor").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::ACTORS, this->ui->actor_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->actor_list, RPGDB::ACTORS, true, 3, false);
    });
    this->ui->actor_list->addAction(&action_actors_paste);


    //CLASSES: CTRL+C  -- CTRL+V
    action_classes_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_classes_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_classes_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_class", QJsonDocument(ec->get_object_by_id(RPGDB::CLASSES,this->ui->classes_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->classes_list->addAction(&action_classes_copy);

    action_classes_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_classes_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_classes_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_class").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::CLASSES, this->ui->classes_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->classes_list, RPGDB::CLASSES, true, 3, false);
    });
    this->ui->classes_list->addAction(&action_classes_paste);



    //SKILLS: CTRL+C  -- CTRL+V
    action_skills_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_skills_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_skills_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_skill", QJsonDocument(ec->get_object_by_id(RPGDB::SKILLS,this->ui->skills_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->skills_list->addAction(&action_skills_copy);

    action_skills_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_skills_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_skills_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_skill").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::SKILLS, this->ui->skills_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->skills_list, RPGDB::SKILLS, true, 3, false);
    });
    this->ui->skills_list->addAction(&action_skills_paste);


    //ITEMS: CTRL+C  -- CTRL+V
    action_items_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_items_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_items_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_item", QJsonDocument(ec->get_object_by_id(RPGDB::ITEMS,this->ui->items_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->items_list->addAction(&action_items_copy);

    action_items_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_items_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_items_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_item").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::ITEMS, this->ui->items_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->items_list, RPGDB::ITEMS, true, 3, false);
    });
    this->ui->items_list->addAction(&action_items_paste);


    //WEAPONS: CTRL+C  -- CTRL+V
    action_weapons_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_weapons_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_weapons_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_weapon", QJsonDocument(ec->get_object_by_id(RPGDB::WEAPONS,this->ui->weapons_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->weapons_list->addAction(&action_weapons_copy);

    action_weapons_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_weapons_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_weapons_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_weapon").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::WEAPONS, this->ui->weapons_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->weapons_list, RPGDB::WEAPONS, true, 3, false);
    });
    this->ui->weapons_list->addAction(&action_weapons_paste);


    //ARMOR: CTRL+C  -- CTRL+V
    action_armors_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_armors_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_armors_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_armor", QJsonDocument(ec->get_object_by_id(RPGDB::ARMORS,this->ui->armors_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->armors_list->addAction(&action_armors_copy);

    action_armors_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_armors_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_armors_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_armor").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::ARMORS, this->ui->armors_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->armors_list, RPGDB::ARMORS, true, 3, false);
    });
    this->ui->armors_list->addAction(&action_armors_paste);


    //ENEMIES: CTRL+C  -- CTRL+V
    action_enemies_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_enemies_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_enemies_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_enemy", QJsonDocument(ec->get_object_by_id(RPGDB::ENEMIES,this->ui->enemies_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->enemies_list->addAction(&action_enemies_copy);

    action_enemies_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_enemies_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_enemies_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_enemy").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::ENEMIES, this->ui->enemies_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->enemies_list, RPGDB::ENEMIES, true, 3, false);
    });
    this->ui->enemies_list->addAction(&action_enemies_paste);

    //TROOPS: CTRL+C  -- CTRL+V
    action_troops_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_troops_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_troops_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_troop", QJsonDocument(ec->get_object_by_id(RPGDB::TROOPS,this->ui->troops_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->troops_list->addAction(&action_troops_copy);

    action_troops_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_troops_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_troops_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_troop").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::TROOPS, this->ui->troops_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->troops_list, RPGDB::TROOPS, true, 3, false);
    });
    this->ui->troops_list->addAction(&action_troops_paste);




    //STATES: CTRL+C  -- CTRL+V
    action_states_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_states_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_states_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_state", QJsonDocument(ec->get_object_by_id(RPGDB::STATES,this->ui->states_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->states_list->addAction(&action_states_copy);

    action_states_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_states_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_states_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_state").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::STATES, this->ui->states_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->states_list, RPGDB::STATES, true, 3, false);
    });
    this->ui->states_list->addAction(&action_states_paste);


    //ANIMATIONS: CTRL+C  -- CTRL+V
    action_animations_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_animations_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_animations_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_animation", QJsonDocument(ec->get_object_by_id(RPGDB::ANIMATIONS,this->ui->animations_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->animations_list->addAction(&action_animations_copy);

    action_animations_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_animations_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_animations_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_animation").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::ANIMATIONS, this->ui->animations_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->animations_list, RPGDB::ANIMATIONS, true, 3, false);
    });
    this->ui->animations_list->addAction(&action_animations_paste);


    //TILESETS: CTRL+C  -- CTRL+V
    action_tilesets_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_tilesets_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_tilesets_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_tilesets", QJsonDocument(ec->get_object_by_id(RPGDB::TILESETS,this->ui->tilesets_list->currentRow()+1)).toJson(QJsonDocument::Compact));
        settings.setValue("copied_tilesets_path", db->tileset_dir);
        settings.setValue("copied_autotiles_path", db->autotiles_dir);
    });
    this->ui->tilesets_list->addAction(&action_tilesets_copy);

    action_tilesets_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_tilesets_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_tilesets_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_tilesets").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;

        if (settings.value("copied_tilesets_path").toString() != db->tileset_dir)
        {
            int res = QMessageBox::question(this, "Import Tileset Graphic Files", "Do you want to import the graphic files of the other project?");
            if (res == QMessageBox::Yes)
            {
                QFile file(FileOpener(settings.value("copied_tilesets_path").toString(), doc.object().value("@tileset_name").toString()).get_image_path());
                file.copy(db->tileset_dir + QFileInfo(file.fileName()).fileName());

                for (int i = 0; i < 7; i++)
                {
                    QFile file(FileOpener(settings.value("copied_autotiles_path").toString(), doc.object().value("@autotile_names").toArray().at(i).toString()).get_image_path());
                    file.copy(db->autotiles_dir + QFileInfo(file.fileName()).fileName());
                }
            }
        }
        ec->set_object_by_id(RPGDB::TILESETS, this->ui->tilesets_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->tilesets_list, RPGDB::TILESETS, true, 3, false);
    });
    this->ui->tilesets_list->addAction(&action_tilesets_paste);



    //COMMONEVENTS: CTRL+C  -- CTRL+V
    action_common_events_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_common_events_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_common_events_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_common_event", QJsonDocument(ec->get_object_by_id(RPGDB::COMMONEVENTS,this->ui->common_events_list->currentRow()+1)).toJson(QJsonDocument::Compact));
    });
    this->ui->common_events_list->addAction(&action_common_events_copy);

    action_common_events_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_common_events_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_common_events_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_common_event").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->set_object_by_id(RPGDB::COMMONEVENTS, this->ui->common_events_list->currentRow()+1, doc.object());
        this->ec->fill_list(this->ui->common_events_list, RPGDB::COMMONEVENTS, true, 3, false);
    });
    this->ui->common_events_list->addAction(&action_common_events_paste);



    //SCRIPTS: CTRL+C  -- CTRL+V
    action_scripts_copy.setShortcut(QKeySequence(tr("Ctrl+C")));
    action_scripts_copy.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_scripts_copy, &QAction::triggered, [=](){
        QSettings settings;
        settings.setValue("copied_script", QJsonDocument(ec->get_script_by_id(this->ui->scripts_list->currentRow())).toJson(QJsonDocument::Compact));
    });
    this->ui->scripts_list->addAction(&action_scripts_copy);

    action_scripts_paste.setShortcut(QKeySequence(tr("Ctrl+V")));
    action_scripts_paste.setShortcutContext(Qt::WidgetShortcut);
    connect(&action_scripts_paste, &QAction::triggered, [=](){
        QSettings settings;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(settings.value("copied_script").toByteArray(), &error);
        if (error.error != QJsonParseError::NoError)
            return;
        ec->add_new_script_at_id(this->ui->scripts_list->currentRow());
        ec->set_object_by_id(RPGDB::SCRIPTS, this->ui->scripts_list->currentRow(), doc.array());
        this->ec->fill_list(this->ui->scripts_list, RPGDB::SCRIPTS, true, 3, false);
    });
    this->ui->scripts_list->addAction(&action_scripts_paste);
}

void DataEditor::set_tileset_and_tile(int tileset_id, int y_value)
{
    this->set_widget(DataEditor::TILESETS);
    this->ui->tilesets_list->setCurrentRow(tileset_id-1);
    this->ui->tilesets_widget->focus_on_tile(y_value);
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
    if (this->set_maximum(RPGDB::ACTORS))
        this->ec->fill_list(this->ui->actor_list, RPGDB::ACTORS, true, 3, false);
}


void DataEditor::on_button_classes_max_clicked()
{
    if (this->set_maximum(RPGDB::CLASSES))
        this->ec->fill_list(this->ui->classes_list, RPGDB::CLASSES, true, 3, false);
}


void DataEditor::on_button_skills_max_clicked()
{
    if (this->set_maximum(RPGDB::SKILLS))
        this->ec->fill_list(this->ui->skills_list, RPGDB::SKILLS, true, 3, false);
}


void DataEditor::on_button_items_max_clicked()
{
    if (this->set_maximum(RPGDB::ITEMS))
        this->ec->fill_list(this->ui->items_list, RPGDB::ITEMS, true, 3, false);
}


void DataEditor::on_button_weapons_max_clicked()
{
    if (this->set_maximum(RPGDB::WEAPONS))
        this->ec->fill_list(this->ui->weapons_list, RPGDB::WEAPONS, true, 3, false);
}


void DataEditor::on_button_armors_max_clicked()
{
    if (this->set_maximum(RPGDB::ARMORS))
        this->ec->fill_list(this->ui->armors_list, RPGDB::ARMORS, true, 3, false);
}


void DataEditor::on_button_enemies_max_clicked()
{
    if (this->set_maximum(RPGDB::ENEMIES))
        this->ec->fill_list(this->ui->enemies_list, RPGDB::ENEMIES, true, 3, false);
}


void DataEditor::on_button_troops_max_clicked()
{
    if (this->set_maximum(RPGDB::TROOPS))
        this->ec->fill_list(this->ui->troops_list, RPGDB::TROOPS, true, 3, false);
}


void DataEditor::on_button_states_max_clicked()
{
    this->set_maximum(RPGDB::STATES);
    this->ec->fill_list(this->ui->states_list, RPGDB::STATES, true, 3, false);
}


void DataEditor::on_button_animations_max_clicked()
{
    if (this->set_maximum(RPGDB::ANIMATIONS))
        this->ec->fill_list(this->ui->animations_list, RPGDB::ANIMATIONS, true, 3, false);
}


void DataEditor::on_button_tilesets_max_clicked()
{
    if (this->set_maximum(RPGDB::TILESETS))
        this->ec->fill_list(this->ui->tilesets_list, RPGDB::TILESETS, true, 3, false);
}


void DataEditor::on_button_common_events_max_clicked()
{
    if (this->set_maximum(RPGDB::COMMONEVENTS))
        this->ec->fill_list(this->ui->common_events_list, RPGDB::COMMONEVENTS, true, 3, false);
}

bool DataEditor::set_maximum(int obj_type)
{
    bool ok = false;
    int max = QInputDialog::getInt(this, "Select Maximum", "Set Maximum",this->ec->count_objects(obj_type), 0, 999,1, &ok);
    if (ok)
    {
        this->ec->set_max(obj_type, max);
        this->ec->refresh(obj_type);
        return true;
    }
    return false;
}

void DataEditor::on_scripts_list_doubleClicked(const QModelIndex &index)
{
    QJsonArray array = ec->get_script_by_id(index.row());
    bool ok = false;
    QString name = QInputDialog::getText(this, "Change Script Name", "Script Name:", QLineEdit::Normal, QString(QByteArray::fromBase64(array.at(1).toString().toUtf8())), &ok);
    if (ok)
    {
        array.removeAt(1);
        array.insert(1, QString(name.toUtf8().toBase64()));
        ec->set_object_by_id(RPGDB::SCRIPTS,index.row(),array);
        this->ec->fill_list(this->ui->scripts_list, RPGDB::SCRIPTS, true, 3, false);
    }
}


void DataEditor::on_button_remove_script_clicked()
{
    ec->remove_script_by_id(ec->current_script);
    this->ec->fill_list(this->ui->scripts_list, RPGDB::SCRIPTS, true, 3, false);
}


void DataEditor::on_buttton_add_script_clicked()
{
    ec->add_new_script_at_id(ec->current_script);
    this->ec->fill_list(this->ui->scripts_list, RPGDB::SCRIPTS, true, 3, false);
}

