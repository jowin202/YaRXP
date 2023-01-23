#ifndef SCROLLMAPDIALOG_H
#define SCROLLMAPDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

namespace Ui {
class ScrollMapDialog;
}

class ScrollMapDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ScrollMapDialog(QJsonArray parameters, QWidget *parent = nullptr);
    ~ScrollMapDialog();

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
    Ui::ScrollMapDialog *ui;
};

#endif // SCROLLMAPDIALOG_H
