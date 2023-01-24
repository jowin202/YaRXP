#ifndef CHANGEACTORGRAPHICDIALOG_H
#define CHANGEACTORGRAPHICDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

class RPGDB;

namespace Ui {
class ChangeActorGraphicDialog;
}

class ChangeActorGraphicDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChangeActorGraphicDialog(RPGDB *db, QJsonArray parameter, QWidget *parent = nullptr);
    ~ChangeActorGraphicDialog();

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

    void on_button_character_clicked();


    void on_button_battler_clicked();

private:
    Ui::ChangeActorGraphicDialog *ui;
    int hue1, hue2;
    RPGDB *db;
};

#endif // CHANGEACTORGRAPHICDIALOG_H
