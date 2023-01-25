#ifndef TRANSFERPLAYERDIALOG_H
#define TRANSFERPLAYERDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

class RPGDB;

namespace Ui {
class TransferPlayerDialog;
}

class TransferPlayerDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TransferPlayerDialog(RPGDB *db, QJsonArray parameters, QWidget *parent = nullptr);
    ~TransferPlayerDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


    void set_direct(int id, int x, int y);
signals:
    void ok_clicked(QJsonArray);
private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

    void on_button_change_location_clicked();

private:
    Ui::TransferPlayerDialog *ui;

    int x = 0,y = 0, id = 0;
    RPGDB *db;
};

#endif // TRANSFERPLAYERDIALOG_H
