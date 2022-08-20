#include "maptreewidget.h"

MapTreeWidget::MapTreeWidget(QWidget *parent) : QTreeWidget(parent)
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
    for (int i = 0; i < system->map_info_list.length(); i++)
    {
        if (system->map_info_list.at(i)->parent_id == 0)
        {
            QStringList columns;
            columns << system->map_info_list.at(i)->name;
            columns << QString::number(system->map_info_list.at(i)->order).rightJustified(3,'0');
            columns << QString::number(i);
            columns << QString::number(system->map_info_list.at(i)->id);

            QTreeWidgetItem *item = new QTreeWidgetItem(columns);
            this->id_map.insert(system->map_info_list.at(i)->id, item);
            this->addTopLevelItem(item);
        }
    }

    do
    {
        for (int i = 0; i < system->map_info_list.length(); i++)
        {
            if (system->map_info_list.at(i)->parent_id != 0)
            {
                QStringList columns;
                columns << system->map_info_list.at(i)->name;
                columns << QString::number(system->map_info_list.at(i)->order).rightJustified(3,'0');
                columns << QString::number(i);
                columns << QString::number(system->map_info_list.at(i)->id);

                QTreeWidgetItem *item = new QTreeWidgetItem(columns);
                //parent ID exists, id doesnt exist yet
                if (this->id_map.value(system->map_info_list.at(i)->parent_id, 0) != 0 && this->id_map.value(system->map_info_list.at(i)->id, 0) == 0)
                {
                    this->id_map.insert(system->map_info_list.at(i)->id, item);
                    this->id_map.value(system->map_info_list.at(i)->parent_id, 0)->addChild(item);
                }
            }
        }
    }
    while(system->map_info_list.size() != id_map.size());
    this->sortItems(1,Qt::SortOrder::AscendingOrder);


}

void MapTreeWidget::clicked_at_item(QTreeWidgetItem *current_item, QTreeWidgetItem* previous)
{
    int list_id = current_item->text(2).toInt(); //id (2nd column) which it has in the list
    QString file_id = current_item->text(3); //id of the file, used as string

    if (system->map_info_list.at(list_id)->map == 0) // do not reload the map (changes should be saved)
    {
        RPGMap *map = new RPGMap();
        try {
            IOMapFile iomap_file(system->data_dir +  "Map" + file_id.rightJustified(3,'0') + ".rxdata", map);
            system->map_info_list.at(list_id)->map = map;
            map->load_event_graphics(this->system);

        } catch (ParserException *ex) {
            this->handleParserException(ex);
            return;
        }




        int tileset_id = map->tileset_id;
        if (!this->system->tileset_hash.contains(tileset_id))
        {
            //error tileset
            QMessageBox::critical(this,"Error", QString("Error: Tileset id %1 not found at predefined tilesets").arg(tileset_id));
            return; //
        }


    }



    emit on_map_selected(system->map_info_list.at(list_id)->map);
    emit on_tileset_changed(system->map_info_list.at(list_id)->map->tileset_id);
}

void MapTreeWidget::prepare_context_menu( const QPoint & pos )
{
    menu.exec(this->mapToGlobal(pos));
}

void MapTreeWidget::show_map_properties_dialog()
{
    if (this->selectedItems().size() <= 0)
        return;
    RPGMapInfo *mapinfo = system->map_info_list.at(this->selectedItems().at(0)->text(2).toInt());
    MapPropertiesDialog *dialog = new MapPropertiesDialog(mapinfo, this->system, 0);
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
