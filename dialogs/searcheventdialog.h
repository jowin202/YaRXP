#ifndef SEARCHEVENTDIALOG_H
#define SEARCHEVENTDIALOG_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class SearchEventDialog;
}

class RPGDB;

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

private:
    Ui::SearchEventDialog *ui;
    RPGDB *db;
};

#endif // SEARCHEVENTDIALOG_H
