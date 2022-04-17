#ifndef MAPTREEWIDGET_H
#define MAPTREEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QDir>
#include <QMessageBox>

#include <QDebug>

class MapTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    MapTreeWidget(QWidget* parent = 0);

public slots:
    void list_maps(QString project_dir);

private:
    QString project_dir;
};

#endif // MAPTREEWIDGET_H
