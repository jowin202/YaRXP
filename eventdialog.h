#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QWidget>
#include "eventpage.h"

namespace Ui {
class EventDialog;
}

class EventDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EventDialog(QWidget *parent = nullptr);
    ~EventDialog();

public slots:
    void do_numbers_of_tabs_right();

private slots:
    void on_button_new_clicked();

    void on_button_delete_clicked();

private:
    Ui::EventDialog *ui;
};

#endif // EVENTDIALOG_H
