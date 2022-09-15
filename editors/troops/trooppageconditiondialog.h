#ifndef TROOPPAGECONDITIONDIALOG_H
#define TROOPPAGECONDITIONDIALOG_H

#include <QWidget>
#include <QKeyEvent>

class RPGSystem;

namespace Ui {
class TroopPageConditionDialog;
}

class TroopPageConditionDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TroopPageConditionDialog(RPGSystem *system, QWidget *parent = nullptr);
    ~TroopPageConditionDialog();


    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::TroopPageConditionDialog *ui;
};

#endif // TROOPPAGECONDITIONDIALOG_H
