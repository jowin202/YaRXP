#ifndef CHANGECOLORTONEDIALOG_H
#define CHANGECOLORTONEDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QKeyEvent>

class RPGDB;

namespace Ui {
class ChangeColorToneDialog;
}

class ChangeColorToneDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeColorToneDialog(RPGDB *db, int code, QJsonArray parameters, QWidget *parent = nullptr);
    ~ChangeColorToneDialog();


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
    Ui::ChangeColorToneDialog *ui;
    int code;
};

#endif // CHANGECOLORTONEDIALOG_H
