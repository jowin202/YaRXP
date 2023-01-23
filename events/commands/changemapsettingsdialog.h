#ifndef CHANGEMAPSETTINGSDIALOG_H
#define CHANGEMAPSETTINGSDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>


class RPGDB;

namespace Ui {
class ChangeMapSettingsDialog;
}

class ChangeMapSettingsDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeMapSettingsDialog(RPGDB *db, QJsonArray parameters, QWidget *parent = nullptr);
    ~ChangeMapSettingsDialog();

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

    void on_button_panorama_clicked();


    void on_button_fog_clicked();

    void on_button_battleback_clicked();

private:
    Ui::ChangeMapSettingsDialog *ui;
    RPGDB *db;


    int hue = 0;
    int opacity = 255;
    int blending = 0;
    int zoom = 200;
    int sx = 0;
    int sy = 0;
};

#endif // CHANGEMAPSETTINGSDIALOG_H
