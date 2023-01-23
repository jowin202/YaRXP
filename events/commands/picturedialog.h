#ifndef PICTUREDIALOG_H
#define PICTUREDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonArray>

class RPGDB;

namespace Ui {
class PictureDialog;
}

class PictureDialog : public QWidget
{
    Q_OBJECT

public:
    explicit PictureDialog(RPGDB *db, int code, QJsonArray parameters, QWidget *parent = nullptr);
    ~PictureDialog();
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

    void on_change_pic_clicked();

private:
    Ui::PictureDialog *ui;
    int code;
    RPGDB *db;
};

#endif // PICTUREDIALOG_H
