#ifndef CHANGETEXTOPTIONSDIALOG_H
#define CHANGETEXTOPTIONSDIALOG_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QKeyEvent>

namespace Ui {
class ChangeTextOptionsDialog;
}

class ChangeTextOptionsDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeTextOptionsDialog(QWidget *parent = nullptr);
    ~ChangeTextOptionsDialog();

    void setParameters(QJsonArray parameters);

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
    Ui::ChangeTextOptionsDialog *ui;
};

#endif // CHANGETEXTOPTIONSDIALOG_H
