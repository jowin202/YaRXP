#include "maptreewidget.h"

MapTreeWidget::MapTreeWidget(QWidget *parent)
{
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(clicked_at_item(QTreeWidgetItem*,int)));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(prepare_context_menu(QPoint)));


    action1.setText("&Map properties");
    action1.setShortcut(QKeySequence(Qt::Key_Space));
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


    menu.addAction(&action1);
    menu.addSeparator();
    menu.addAction(&action2);
    menu.addSeparator();
    menu.addAction(&action3);
    menu.addAction(&action4);
    menu.addAction(&action5);
    menu.addSeparator();
    menu.addAction(&action6);

}

void MapTreeWidget::list_maps(QString project_dir)
{
    if (project_dir != "")
    {
        this->project_dir = project_dir;
        this->map_info_file = project_dir + QDir::separator() + "Data" + QDir::separator() + "MapInfos.rxdata";
    }

    RXDataParser parser(this->map_info_file);
    parser.parseMapInfo(&this->map_list);

    for (int i = 0; i < map_list.length(); i++)
    {
        if (map_list.at(i)->parent_id == 0)
        {
            QStringList columns;
            columns << map_list.at(i)->name;
            columns << QString::number(map_list.at(i)->order).rightJustified(3,'0');

            QTreeWidgetItem *item = new QTreeWidgetItem(columns);
            this->id_map.insert(map_list.at(i)->id, item);
            this->addTopLevelItem(item);
        }
    }

    int old_size;
    do
    {
        old_size = map_list.size();
        for (int i = 0; i < map_list.length(); i++)
        {
            if (map_list.at(i)->parent_id != 0)
            {
                QStringList columns;
                columns << map_list.at(i)->name;
                columns << QString::number(map_list.at(i)->order).rightJustified(3,'0');

                QTreeWidgetItem *item = new QTreeWidgetItem(columns);
                //parent ID exists, id doesnt exist yet
                if (this->id_map.value(map_list.at(i)->parent_id, 0) != 0 && this->id_map.value(map_list.at(i)->id, 0) == 0)
                {
                    this->id_map.insert(map_list.at(i)->id, item);
                    this->id_map.value(map_list.at(i)->parent_id, 0)->addChild(item);
                }
            }
        }
    }
    while(old_size != id_map.size());
    this->sortItems(1,Qt::SortOrder::AscendingOrder);

    /*
    QDir map_dir(this->project_dir + QDir::separator() + "Data");
    if (!map_dir.exists())
    {
        QMessageBox::critical(this, "No Data Dir", "No Data dir (Case-sensitive)");
        return;
    }

    QStringList files = map_dir.entryList();
    QRegExp re("^(Map)(\\d+)(.rxdata)$");
    QStringList files_filtered = files.filter(re);

    for (int i = 0; i < files_filtered.length(); i++)
    {
        if (files_filtered.at(i).startsWith("Map"))
        {
            this->addTopLevelItem(new QTreeWidgetItem(QStringList(files_filtered.at(i))));
        }
    }
    */
}

void MapTreeWidget::clicked_at_item(QTreeWidgetItem *item, int column)
{
    RXDataParser parser(project_dir + QDir::separator() + "Data" + QDir::separator() + item->text(0));
    parser.parse();
}

void MapTreeWidget::prepare_context_menu( const QPoint & pos )
{
    qDebug() << "test";

    menu.exec(this->mapToGlobal(pos));
}
