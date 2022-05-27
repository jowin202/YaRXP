#ifndef EVENTCOMMANDDIALOG_H
#define EVENTCOMMANDDIALOG_H

#include <QWidget>

namespace Ui {
class EventCommandDialog;
}

class EventCommandDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EventCommandDialog(QWidget *parent = nullptr);
    ~EventCommandDialog();

private:
    Ui::EventCommandDialog *ui;
};

#endif // EVENTCOMMANDDIALOG_H
