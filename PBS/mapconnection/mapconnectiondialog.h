#ifndef MAPCONNECTIONDIALOG_H
#define MAPCONNECTIONDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QTreeWidgetItem>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QKeyEvent>

#include <QTimer>
#include <QJsonDocument>
#include <QMessageBox>

#include "link.h"

class RPGDB;
class MapGraphicsItem;


struct cached_map{
    QImage img;
    QList<link> links;
};


namespace Ui {
class MapConnectionDialog;
}

class MapConnectionDialog : public QWidget
{
    Q_OBJECT

public:
    explicit MapConnectionDialog(RPGDB *db, QWidget *parent = nullptr);
    ~MapConnectionDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

    void list_maps();
    void display_maps(int id, bool center = true);

    MapGraphicsItem *render_map(int id);


signals:

public slots:
    void jump_to_item(int val);

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_button_refresh_clicked();


    void on_button_up_clicked();
    void on_button_down_clicked();
    void on_button_left_clicked();
    void on_button_right_clicked();

private:
    Ui::MapConnectionDialog *ui;

    QMap<int,cached_map> cached_maps;

    QJsonDocument connection_file;
    RPGDB *db;
    QHash<int,QTreeWidgetItem*> id_map;
    int current_id = -1;
};

#endif // MAPCONNECTIONDIALOG_H
