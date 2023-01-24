#ifndef FORCEACTIONDIALOG_H
#define FORCEACTIONDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonArray>

class RPGDB;

namespace Ui {
class ForceActionDialog;
}

class ForceActionDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ForceActionDialog(RPGDB *db, QJsonArray parameters, QWidget *parent = nullptr);
    ~ForceActionDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked(QJsonArray);


private slots:
    void on_button_cancel_clicked();

    void on_button_ok_clicked();

private:
    Ui::ForceActionDialog *ui;
};

#endif // FORCEACTIONDIALOG_H
