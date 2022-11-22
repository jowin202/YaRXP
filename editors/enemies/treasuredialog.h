#ifndef TREASUREDIALOG_H
#define TREASUREDIALOG_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class TreasureDialog;
}

class RPGSystem;
class TreasureDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TreasureDialog(RPGSystem *system, int item_id, int weapon_id, int armor_id, int prob, QWidget *parent = nullptr);
    ~TreasureDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked(int,int,int,int);
private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::TreasureDialog *ui;
};

#endif // TREASUREDIALOG_H