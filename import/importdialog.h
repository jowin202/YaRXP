#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QWidget>
#include <QPainter>
#include <QTreeWidgetItem>
#include <QFileDialog>

class RPGDB;
namespace Ui {
class ImportDialog;
}

class ImportDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ImportDialog(RPGDB *db, QWidget *parent = nullptr);
    ~ImportDialog();

    void list_maps();
    void display_map();

private slots:
    void on_button_browse_clicked();

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

private:
    Ui::ImportDialog *ui;
    RPGDB *db;
    RPGDB *secondary_db = 0;

    int id;
    QHash<int,QTreeWidgetItem*> id_map;
};

#endif // IMPORTDIALOG_H
