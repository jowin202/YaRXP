#ifndef SEARCHEVENTDIALOG_H
#define SEARCHEVENTDIALOG_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class SearchEventDialog;
}

class RPGDB;
class RPGMapInfoController;

class SearchEventDialog : public QWidget
{
    Q_OBJECT

    void keyPressEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
        else if (event->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
    }


public:
    explicit SearchEventDialog(RPGDB *db);
    ~SearchEventDialog();

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();
    void update_gui(QList<int> result);
    void on_table_result_cellDoubleClicked(int row, int column);

private:
    Ui::SearchEventDialog *ui;
    RPGDB *db;
    RPGMapInfoController *mic;

signals:
    void select_event(int,int,int,int);
};

#endif // SEARCHEVENTDIALOG_H
