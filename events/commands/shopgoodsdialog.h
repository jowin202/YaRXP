#ifndef SHOPGOODSDIALOG_H
#define SHOPGOODSDIALOG_H

#include <QWidget>
#include <QKeyEvent>

class RPGDB;

namespace Ui {
class ShopGoodsDialog;
}

class ShopGoodsDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ShopGoodsDialog(RPGDB *db, int type, int current, QWidget *parent = nullptr);
    ~ShopGoodsDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked(int,int);

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();


private:
    Ui::ShopGoodsDialog *ui;
};

#endif // SHOPGOODSDIALOG_H
