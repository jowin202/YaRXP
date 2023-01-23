#ifndef CHANGEEQUIPMENTDIALOG_H
#define CHANGEEQUIPMENTDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

class RPGDB;

namespace Ui {
class ChangeEquipmentDialog;
}

class ChangeEquipmentDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeEquipmentDialog(RPGDB *db, QJsonArray parameters, QWidget *parent = nullptr);
    ~ChangeEquipmentDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


    void update_combos();
signals:
    void ok_clicked(QJsonArray);

private slots:
    void on_button_cancel_clicked();
    void on_button_ok_clicked();
    void on_combo_actor_currentIndexChanged(int index);

private:
    Ui::ChangeEquipmentDialog *ui;
    RPGDB *db;
};

#endif // CHANGEEQUIPMENTDIALOG_H
