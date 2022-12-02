#ifndef ENEMYACTIONDIALOG_H
#define ENEMYACTIONDIALOG_H

#include <QWidget>
#include <QKeyEvent>


namespace Ui {
class EnemyActionDialog;
}

class RPGEditorController;

class EnemyActionDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EnemyActionDialog(RPGEditorController *ec, int row, int turn_a, int turn_b, int hp, int level, int switch_id, int kind, int rating, int skill, int basic, QWidget *parent = nullptr);
    ~EnemyActionDialog();


    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


signals:
    void ok_clicked(int, int, int, int, int, int , int, int, int, int);

private slots:
    void on_button_cancel_clicked();
    void on_button_ok_clicked();

private:
    Ui::EnemyActionDialog *ui;
    RPGEditorController *ec;
    int row;
};

#endif // ENEMYACTIONDIALOG_H
