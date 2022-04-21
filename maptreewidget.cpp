#include "maptreewidget.h"

MapTreeWidget::MapTreeWidget(QWidget *parent)
{
    connect(this, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(clicked_at_item(QTreeWidgetItem*,int)));
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
            QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(map_list.at(i)->name));
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
                QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(map_list.at(i)->name));
                if (this->id_map.value(map_list.at(i)->parent_id, 0) != 0)
                {
                    this->id_map.insert(map_list.at(i)->id, item);
                    this->id_map.value(map_list.at(i)->parent_id, 0)->addChild(item);
                }
            }
        }
    }
    while(old_size != id_map.size());

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
