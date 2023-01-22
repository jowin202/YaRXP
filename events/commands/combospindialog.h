#ifndef COMBOSPINDIALOG_H
#define COMBOSPINDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

class RPGDB;

namespace Ui {
class ComboSpinDialog;
}

class ComboSpinDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ComboSpinDialog(RPGDB *db, int code, QJsonArray parameters, QWidget *parent = nullptr);
    ~ComboSpinDialog();


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
    Ui::ComboSpinDialog *ui;
};

#endif // COMBOSPINDIALOG_H
