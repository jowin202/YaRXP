#ifndef SCREENSHAKEDIALOG_H
#define SCREENSHAKEDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

namespace Ui {
class ScreenShakeDialog;
}

class ScreenShakeDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenShakeDialog(QJsonArray parameters, QWidget *parent = nullptr);
    ~ScreenShakeDialog();

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
    Ui::ScreenShakeDialog *ui;
};

#endif // SCREENSHAKEDIALOG_H
