#ifndef LISTDIALOG_H
#define LISTDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QInputDialog>
#include <QListWidgetItem>

namespace Ui {
class ListDialog;
}

class RPGSystem;

class ListDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ListDialog(RPGSystem *system, QWidget *parent = nullptr);
    ~ListDialog();

    void switch_dialog();
    void variable_dialog();
    int getValue();
    void setValue(int value);


    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


signals:
    void ok_clicked(int);
private slots:
    void on_button_cancel_clicked();
    void on_button_ok_clicked();

    void on_button_change_max_clicked();

    void on_list_doubleClicked(const QModelIndex &index);

private:
    Ui::ListDialog *ui;
    RPGSystem *system;

    bool switch_is_set = false;
    bool variable_is_set = false;
};

#endif // LISTDIALOG_H
