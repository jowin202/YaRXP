#ifndef CHOICESDIALOG_H
#define CHOICESDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

namespace Ui {
class ChoicesDialog;
}

class ChoicesDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChoicesDialog(QJsonArray parameters, QWidget *parent = nullptr);
    ~ChoicesDialog();
    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked(QJsonArray);

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::ChoicesDialog *ui;
};

#endif // CHOICESDIALOG_H
