#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QKeyEvent>
#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include "eventpage.h"

class RPGMapController;


namespace Ui {
class EventDialog;
}

class EventDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EventDialog(RPGMapController *mc, QJsonObject event, QWidget *parent = nullptr);
    ~EventDialog();
    void keyPressEvent(QKeyEvent *event)
    {
        //if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        //    this->on_button_ok_clicked();
        if (event->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
    }

public slots:
    void do_numbers_of_tabs_right();

private slots:
    void on_button_new_clicked();
    void on_button_delete_clicked();
    void on_button_ok_clicked();
    void on_button_cancel_clicked();
    void on_button_apply_clicked();

private:
    Ui::EventDialog *ui;
    QJsonObject event;
    RPGMapController *mc;
};

#endif // EVENTDIALOG_H
