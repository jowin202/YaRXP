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
}

void MapTreeWidget::list_maps(QString project_dir)
{
    if (project_dir != "")
    {
        this->project_dir = project_dir;
        this->map_info_file = project_dir + QDir::separator() + "Data" + QDir::separator() + "MapInfos.rxdata";
        this->tileset_file = project_dir + QDir::separator() + "Data" + QDir::separator() + "Tilesets.rxdata";
    }

    RXDataParser parser(this->map_info_file);
    parser.parseMapInfo(&this->map_info_list);

    for (int i = 0; i < map_info_list.length(); i++)
    {
        if (map_info_list.at(i)->parent_id == 0)
        {
            QStringList columns;
            columns << map_info_list.at(i)->name;
            columns << QString::number(map_info_list.at(i)->order).rightJustified(3,'0');
            columns << QString::number(i);
            columns << QString::number(map_info_list.at(i)->id);

            QTreeWidgetItem *item = new QTreeWidgetItem(columns);
            this->id_map.insert(map_info_list.at(i)->id, item);
            this->addTopLevelItem(item);
        }
    }

    int old_size;
    do
    {
        old_size = map_info_list.size();
        for (int i = 0; i < map_info_list.length(); i++)
        {
            if (map_info_list.at(i)->parent_id != 0)
            {
                QStringList columns;
                columns << map_info_list.at(i)->name;
                columns << QString::number(map_info_list.at(i)->order).rightJustified(3,'0');
                columns << QString::number(i);
                columns << QString::number(map_info_list.at(i)->id);

                QTreeWidgetItem *item = new QTreeWidgetItem(columns);
                //parent ID exists, id doesnt exist yet
                if (this->id_map.value(map_info_list.at(i)->parent_id, 0) != 0 && this->id_map.value(map_info_list.at(i)->id, 0) == 0)
                {
                    this->id_map.insert(map_info_list.at(i)->id, item);
                    this->id_map.value(map_info_list.at(i)->parent_id, 0)->addChild(item);
                }
            }
        }
    }
    while(old_size != id_map.size());
    this->sortItems(1,Qt::SortOrder::AscendingOrder);


    RXDataParser parser2(tileset_file);
    parser2.parseTilesetList(this->tilesets);

}

void MapTreeWidget::clicked_at_item(QTreeWidgetItem *current_item, QTreeWidgetItem* previous)
{
    int list_id = current_item->text(2).toInt(); //id (2nd column) which it has in the list
    QString file_id = current_item->text(3); //id of the file, used as string

    if (map_info_list.at(list_id)->map == 0) // do not reload the map (changes should be saved)
    {
        RXDataParser parser(project_dir + QDir::separator() + "Data" + QDir::separator() + "Map" + file_id.rightJustified(3,'0') + ".rxdata");


        map_info_list.at(list_id)->map = parser.parseMap();
        int tileset_id = map_info_list.at(list_id)->map->tileset_id;
        if (!this->tilesets->contains(tileset_id))
        {
            //error tileset
            qDebug() << "error: tileset not found at predefined tilesets";
            exit(1);
        }
        RPGTileset *current_tileset = this->tilesets->value(tileset_id);

        //Load tileset image (only once)
        if (current_tileset->tileset.isNull())
        {
            if (QFile(this->project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() + current_tileset->tileset_name + ".png").exists())
            {
                current_tileset->tileset = QImage(this->project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() +current_tileset->tileset_name + ".png");
            }
            else if (QFile(this->project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() +current_tileset->tileset_name + ".PNG").exists())
            {
                current_tileset->tileset = QImage(this->project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() +current_tileset->tileset_name + ".PNG");
            }
            else
            {
                qDebug() << "error: tileset does not exist: " << this->project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator() +current_tileset->tileset_name + ".png";
                exit(1);
            }
        }


        //Load tileset autotiles (only once)
        if (current_tileset->autotiles.length() < 7)
        {
            for (int i = 0; i < current_tileset->autotile_names.length(); i++)
            {
                QImage autotiles;
                if (QFile(this->project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() + current_tileset->autotile_names.at(i) + ".png").exists())
                {
                     autotiles = QImage(this->project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() +current_tileset->autotile_names.at(i) + ".png");
                     current_tileset->autotiles.append(Autotileset(autotiles));
                }
                else if (QFile(this->project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() +current_tileset->autotile_names.at(i) + ".PNG").exists())
                {
                     autotiles = QImage(this->project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator() +current_tileset->autotile_names.at(i) + ".PNG");
                     current_tileset->autotiles.append(Autotileset(autotiles));
                }
                else
                {
                    current_tileset->autotiles.append(Autotileset());
                }

            }
        }

        map_info_list.at(list_id)->map->tileset = tilesets->value(tileset_id);

    }
    emit on_map_selected(map_info_list.at(list_id)->map);
    emit on_tileset_changed(map_info_list.at(list_id)->map->tileset_id);
}

void MapTreeWidget::prepare_context_menu( const QPoint & pos )
{
    menu.exec(this->mapToGlobal(pos));
}

void MapTreeWidget::show_map_properties_dialog()
{
    if (this->selectedItems().size() <= 0)
        return;
    RPGMapInfo *mapinfo = map_info_list.at(this->selectedItems().at(0)->text(2).toInt());
    MapProperties *dialog = new MapProperties(mapinfo, tilesets, 0);
    dialog->show();
}
