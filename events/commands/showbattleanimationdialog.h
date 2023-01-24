#ifndef SHOWBATTLEANIMATIONDIALOG_H
#define SHOWBATTLEANIMATIONDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

class RPGDB;

namespace Ui {
class ShowBattleAnimationDialog;
}

class ShowBattleAnimationDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ShowBattleAnimationDialog(RPGDB *db, QJsonArray parameters, QWidget *parent = nullptr);
    ~ShowBattleAnimationDialog();

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
    Ui::ShowBattleAnimationDialog *ui;
};

#endif // SHOWBATTLEANIMATIONDIALOG_H
