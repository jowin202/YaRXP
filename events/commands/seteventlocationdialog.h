#ifndef SETEVENTLOCATIONDIALOG_H
#define SETEVENTLOCATIONDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

class RPGDB;
class RPGMapController;

namespace Ui {
class SetEventLocationDialog;
}

class SetEventLocationDialog : public QWidget
{
    Q_OBJECT

public:
    explicit SetEventLocationDialog(RPGDB *db, RPGMapController *mc, QJsonArray parameters, QWidget *parent = nullptr);
    ~SetEventLocationDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


    void set_direct(int x, int y);
signals:
    void ok_clicked(QJsonArray);

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

    void on_button_change_location_clicked();

private:
    Ui::SetEventLocationDialog *ui;
    RPGDB *db;
    RPGMapController *mc;
    int x = 0, y = 0;
};

#endif // SETEVENTLOCATIONDIALOG_H
