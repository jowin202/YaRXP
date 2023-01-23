#ifndef WEATHERDIALOG_H
#define WEATHERDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonArray>

namespace Ui {
class WeatherDialog;
}

class WeatherDialog : public QWidget
{
    Q_OBJECT

public:
    explicit WeatherDialog(int code, QJsonArray parameters, QWidget *parent = nullptr);
    ~WeatherDialog();
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
    Ui::WeatherDialog *ui;
};

#endif // WEATHERDIALOG_H
