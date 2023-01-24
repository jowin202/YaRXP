#ifndef SHOPPROCESSINGDIALOG_H
#define SHOPPROCESSINGDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

#include <QAction>
#include <QMenu>

class RPGDB;

namespace Ui {
class ShopProcessingDialog;
}

class ShopProcessingDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ShopProcessingDialog(RPGDB *db, QJsonArray shop_params, QWidget *parent = nullptr);
    ~ShopProcessingDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

public slots:
    void on_customContextMenuRequested(const QPoint &pos);
signals:

    void ok_clicked(QJsonArray);

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();
    void update_table();

    void new_item();
    void new_weapon();
    void new_armor();
    void edit();
    void del();

    void on_button_item_clicked();

    void on_button_weapon_clicked();

    void on_button_armor_clicked();

    void on_button_del_clicked();

private:
    Ui::ShopProcessingDialog *ui;
    QJsonArray shop_params;
    RPGDB *db;

    QAction action_item;
    QAction action_weapon;
    QAction action_armor;
    QAction action_delete;
    QAction action_edit;
};

#endif // SHOPPROCESSINGDIALOG_H
