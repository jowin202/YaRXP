#include "maptreewidget.h"

MapTreeWidget::MapTreeWidget(QWidget *parent)
{
    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(clicked_at_item(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(prepare_context_menu(QPoint)));


    action1.setText("&Map properties");
    action1.setShortcut(QKeySequence(Qt::Key_Space));
    connect(&action1, SIGNAL(triggered()), this, SLOT(show_map_properties_dialog()));
    action2.setText("&New Map");
    action2.setShortcut(QKeySequence(Qt::Key_Insert));
    action3.setText("&Copy");
    action3.setShortcut(QKeySequence(tr("Ctrl+C")));
    action4.setText("&Paste");
    action4.setShortcut(QKeySequence(tr("Ctrl+V")));
    action5.setText("&Delete");
    action5.setShortcut(QKeySequence(Qt::Key_Delete));
    action6.setText("&Shift");
    action6.setShortcut(QKeySequence(tr("Ctrl+T")));
    action7.setText("&Extend");
    action7.setShortcut(QKeySequence(tr("Ctrl+E")));
    action8.setText("&Import");


    menu.addAction(&action1);
    this->addAction(&action1); //for shortcut
    menu.addSeparator();
    menu.addAction(&action2);
    menu.addSeparator();
    menu.addAction(&action3);
    menu.addAction(&action4);
    menu.addAction(&action5);
    menu.addSeparator();
    menu.addAction(&action6);
    menu.addAction(&action7);
    menu.addSeparator();
    menu.addAction(&action8);
}

void MapTreeWidget::list_maps()
{
    try {
        IOMapInfoFile mapinfo_file(settings->data_dir + "MapInfos.rxdata", &settings->map_info_list);
    } catch (ParserException *ex) {
        this->handleParserException(ex);
        settings->map_info_list.clear(); //revert
        //TODO pointer
        return;
    }

    for (int i = 0; i < settings->map_info_list.length(); i++)
    {
        if (settings->map_info_list.at(i)->parent_id == 0)
        {
            QStringList columns;
            columns << settings->map_info_list.at(i)->name;
            columns << QString::number(settings->map_info_list.at(i)->order).rightJustified(3,'0');
            columns << QString::number(i);
            columns << QString::number(settings->map_info_list.at(i)->id);

            QTreeWidgetItem *item = new QTreeWidgetItem(columns);
            this->id_map.insert(settings->map_info_list.at(i)->id, item);
            this->addTopLevelItem(item);
        }
    }

    do
    {
        for (int i = 0; i < settings->map_info_list.length(); i++)
        {
            if (settings->map_info_list.at(i)->parent_id != 0)
            {
                QStringList columns;
                columns << settings->map_info_list.at(i)->name;
                columns << QString::number(settings->map_info_list.at(i)->order).rightJustified(3,'0');
                columns << QString::number(i);
                columns << QString::number(settings->map_info_list.at(i)->id);

                QTreeWidgetItem *item = new QTreeWidgetItem(columns);
                //parent ID exists, id doesnt exist yet
                if (this->id_map.value(settings->map_info_list.at(i)->parent_id, 0) != 0 && this->id_map.value(settings->map_info_list.at(i)->id, 0) == 0)
                {
                    this->id_map.insert(settings->map_info_list.at(i)->id, item);
                    this->id_map.value(settings->map_info_list.at(i)->parent_id, 0)->addChild(item);
                }
            }
        }
    }
    while(settings->map_info_list.size() != id_map.size());
    this->sortItems(1,Qt::SortOrder::AscendingOrder);


    try {
        IOTilesetFile tileset_file(settings->data_dir + "Tilesets.rxdata", &this->settings->tileset_hash, &this->settings->tileset_list);

    } catch (ParserException *ex) {
        this->handleParserException(ex);
        this->settings->tileset_hash.clear(); //TODO pointer
        this->settings->tileset_list.clear(); //TODO pointer
    }

}

void MapTreeWidget::clicked_at_item(QTreeWidgetItem *current_item, QTreeWidgetItem* previous)
{
    int list_id = current_item->text(2).toInt(); //id (2nd column) which it has in the list
    QString file_id = current_item->text(3); //id of the file, used as string

    if (settings->map_info_list.at(list_id)->map == 0) // do not reload the map (changes should be saved)
    {
        RPGMap *map = new RPGMap();
        try {
            IOMapFile iomap_file(settings->data_dir +  "Map" + file_id.rightJustified(3,'0') + ".rxdata", map);
        } catch (ParserException *ex) {
            this->handleParserException(ex);
            return;
        }

        settings->map_info_list.at(list_id)->map = map;
        int tileset_id = map->tileset_id;
        if (!this->settings->tileset_hash.contains(tileset_id))
        {
            //error tileset
            QMessageBox::critical(this,"Error", QString("Error: Tileset id %1 not found at predefined tilesets").arg(tileset_id));
            return; //
        }


        //Load event graphics (only once)
        for (int i = 0; i < map->events.length(); i++)
        {
            for (int j = 0; j < map->events.at(i)->pages.length(); j++)
            {
                if (map->events.at(i)->pages.at(j)->graphics.isNull())
                {
                    QString character_name = map->events.at(i)->pages.at(j)->character_name;
                    if (QFile(settings->characters_dir + character_name + ".png").exists())
                    {
                        map->events.at(i)->pages.at(j)->graphics = QImage(settings->characters_dir + character_name + ".png");
                    }
                    else if (QFile(settings->characters_dir + character_name + ".PNG").exists())
                    {
                        map->events.at(i)->pages.at(j)->graphics = QImage(settings->characters_dir + character_name + ".PNG");

                    }
                }
            }
        }



        //Load tileset image (only once)
        RPGTileset *current_tileset = this->settings->tileset_hash.value(tileset_id);
        if (current_tileset->tileset.isNull() && current_tileset->tileset_name != "") //if tileset_name == "", no tileset specified
        {
            if (QFile(settings->tileset_dir + current_tileset->tileset_name + ".png").exists())
            {
                current_tileset->tileset = QImage(settings->tileset_dir + current_tileset->tileset_name + ".png");
            }
            else if (QFile(settings->tileset_dir + current_tileset->tileset_name + ".PNG").exists())
            {
                current_tileset->tileset = QImage(settings->tileset_dir + current_tileset->tileset_name + ".PNG");
            }
            else
            {
                QMessageBox::critical(this,"Error", QString("Error: Tileset not found:\n%1").arg(settings->tileset_dir + current_tileset->tileset_name + ".png"));
                return;
            }
        }


        //Load tileset autotiles (only once)
        if (current_tileset->autotiles.length() < 7)
        {
            for (int i = 0; i < current_tileset->autotile_names.length(); i++)
            {
                QImage autotiles;
                if (QFile(settings->autotiles_dir + current_tileset->autotile_names.at(i) + ".png").exists())
                {
                     autotiles = QImage(settings->autotiles_dir + current_tileset->autotile_names.at(i) + ".png");
                     current_tileset->autotiles.append(Autotileset(autotiles));
                }
                else if (QFile(settings->autotiles_dir + current_tileset->autotile_names.at(i) + ".PNG").exists())
                {
                     autotiles = QImage(settings->autotiles_dir + current_tileset->autotile_names.at(i) + ".PNG");
                     current_tileset->autotiles.append(Autotileset(autotiles));
                }
                else
                {
                    current_tileset->autotiles.append(Autotileset());
                }
            }
        }


    }
    emit on_map_selected(settings->map_info_list.at(list_id)->map);
    emit on_tileset_changed(settings->map_info_list.at(list_id)->map->tileset_id);
}

void MapTreeWidget::prepare_context_menu( const QPoint & pos )
{
    menu.exec(this->mapToGlobal(pos));
}

void MapTreeWidget::show_map_properties_dialog()
{
    if (this->selectedItems().size() <= 0)
        return;
    RPGMapInfo *mapinfo = settings->map_info_list.at(this->selectedItems().at(0)->text(2).toInt());
    MapPropertiesDialog *dialog = new MapPropertiesDialog(mapinfo, this->settings, 0);
    dialog->show();
}

void MapTreeWidget::handleParserException(ParserException *ex)
{
    if (ex->error_data.length() == 0)
    {
        QMessageBox::critical(this,"Error!", "Unkonwn error!");
    }
    else
        QMessageBox::critical(this,"Error!", QString("Error parsing file: %1\nOffset: %2 (0x%3)\nIn Function: %4\n\n%5")
                    .arg(ex->error_data.at(0).toString())
                    .arg(ex->error_data.at(1).toInt())
                    .arg(QString::number(ex->error_data.at(1).toInt(),16))
                    .arg(ex->error_data.at(2).toString())
                    .arg(ex->message)
                );
}
