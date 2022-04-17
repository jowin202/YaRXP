#include "maptreewidget.h"

MapTreeWidget::MapTreeWidget(QWidget *parent)
{

}

void MapTreeWidget::list_maps(QString project_dir)
{
    if (project_dir != "")
        this->project_dir = project_dir;

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
}
