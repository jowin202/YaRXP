#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QWidget>
#include <QPainter>
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QScrollBar>
#include <QLabel>

#include <QJsonObject>

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
    void display_maps();

    void draw_map_to_label(RPGDB *currentdb, QLabel *target, QJsonObject tileset, QJsonObject map);

    QImage get_autotiles_image(RPGDB *currentdb, QJsonObject tileset);

signals:
    void finished();

private slots:
    void on_button_browse_clicked();
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_button_import_clicked();
    void on_button_adjust_clicked();
private:
    Ui::ImportDialog *ui;
    RPGDB *db;
    RPGDB *secondary_db = 0;

    QJsonObject adjusted_map;

    int id;
    QHash<int,QTreeWidgetItem*> id_map;
};

#endif // IMPORTDIALOG_H
