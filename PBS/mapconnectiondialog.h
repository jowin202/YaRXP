#ifndef MAPCONNECTIONDIALOG_H
#define MAPCONNECTIONDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QTreeWidgetItem>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QKeyEvent>

#include <QJsonDocument>
#include <QMessageBox>

class RPGDB;

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
    void display_maps(int id);

    QImage render_map(int id);


signals:

private slots:
    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::MapConnectionDialog *ui;

    QJsonDocument connection_file;
    RPGDB *db;
    QHash<int,QTreeWidgetItem*> id_map;
    QMap<QString, QStringList> param_oders;
};

#endif // MAPCONNECTIONDIALOG_H
