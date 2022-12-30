#ifndef MAPPROPERTIESDIALOG_H
#define MAPPROPERTIESDIALOG_H

#include <QWidget>
#include <QDebug>
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QInputDialog>

#include <QJsonObject>
#include <QJsonDocument>

class RPGMapInfoController;

namespace Ui {
class MapPropertiesDialog;
}

class MapPropertiesDialog : public QWidget
{
    Q_OBJECT

public:
    explicit MapPropertiesDialog(RPGMapInfoController *mic, int id, QWidget *parent = nullptr);
    ~MapPropertiesDialog();

    void keyPressEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
        else if (event->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
    }


    void set_readonly(QTableWidgetItem *item)
    { item->setFlags(item->flags() & ~Qt::ItemIsEditable); }


    void set_encounter_list_value(int row, int troop);



private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();
    void on_button_bgm_clicked();
    void on_button_bgs_clicked();

    void update_troop(int val);


    void on_table_encounters_itemDoubleClicked(QTableWidgetItem *item);

    void on_button_add_clicked();

    void on_button_del_clicked();

signals:
    void ok_clicked();

private:
    Ui::MapPropertiesDialog *ui;

    int last_edited_troop_row;

    int id;
    RPGMapInfoController *mic;
};

#endif // MAPPROPERTIESDIALOG_H
