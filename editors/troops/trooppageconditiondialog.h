#ifndef TROOPPAGECONDITIONDIALOG_H
#define TROOPPAGECONDITIONDIALOG_H

#include <QDebug>
#include <QWidget>
#include <QKeyEvent>

class RPGEditorController;

namespace Ui {
class TroopPageConditionDialog;
}

class RPGTroopPage;

class TroopPageConditionDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TroopPageConditionDialog(RPGEditorController *ec, QWidget *parent = nullptr);
    ~TroopPageConditionDialog();

    void setPage(int page_num);


    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();


signals:
    void values_changed();

private:
    Ui::TroopPageConditionDialog *ui;
    RPGEditorController *ec;
    int page_num;
};

#endif // TROOPPAGECONDITIONDIALOG_H
