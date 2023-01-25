#ifndef MAPLOCATIONFINDERDIALOG_H
#define MAPLOCATIONFINDERDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QTreeWidgetItem>
#include <QGraphicsPixmapItem>
#include <QPainter>

class RPGDB;

namespace Ui {
class MapLocationFinderDialog;
}

class MapLocationFinderDialog : public QWidget
{
    Q_OBJECT

public:
    explicit MapLocationFinderDialog(RPGDB *db, int id, int x, int y, QWidget *parent = nullptr);
    ~MapLocationFinderDialog();

    void list_maps();


signals:
    void ok_clicked(int,int,int);

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::MapLocationFinderDialog *ui;

    int id, x,y;

    RPGDB *db;
    QHash<int,QTreeWidgetItem*> id_map;
};

#endif // MAPLOCATIONFINDERDIALOG_H
