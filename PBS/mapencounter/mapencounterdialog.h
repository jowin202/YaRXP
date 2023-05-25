#ifndef MAPENCOUNTERDIALOG_H
#define MAPENCOUNTERDIALOG_H

#include <QWidget>
#include <QHash>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QJsonDocument>
#include <QKeyEvent>
#include <QInputDialog>

class RPGDB;

namespace Ui {
class MapEncounterDialog;
}

class MapEncounterDialog : public QWidget
{
    Q_OBJECT

public:
    explicit MapEncounterDialog(RPGDB *db, QWidget *parent = nullptr);
    ~MapEncounterDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
    }



    void list_maps();
    void change_current(int version_index, int map, int version);


private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void combo_version_changed();

    void on_button_cancel_clicked();

    void on_button_ok_clicked();

    void on_button_add_version_clicked();

private:
    Ui::MapEncounterDialog *ui;
    RPGDB *db;
    QJsonDocument encounters_file;
    QHash<int,QTreeWidgetItem*> id_map;
    int current_id;
    int current_map_array_index;
};

#endif // MAPENCOUNTERDIALOG_H
