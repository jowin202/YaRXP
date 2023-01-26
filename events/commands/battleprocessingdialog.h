#ifndef BATTLEPROCESSINGDIALOG_H
#define BATTLEPROCESSINGDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonArray>

class RPGDB;

namespace Ui {
class BattleProcessingDialog;
}

class BattleProcessingDialog : public QWidget
{
    Q_OBJECT

public:
    explicit BattleProcessingDialog(RPGDB *db, QJsonArray parameters, QWidget *parent = nullptr);
    ~BattleProcessingDialog();

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
    Ui::BattleProcessingDialog *ui;
};

#endif // BATTLEPROCESSINGDIALOG_H
