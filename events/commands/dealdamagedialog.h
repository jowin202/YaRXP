#ifndef DEALDAMAGEDIALOG_H
#define DEALDAMAGEDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

class RPGDB;

namespace Ui {
class DealDamageDialog;
}

class DealDamageDialog : public QWidget
{
    Q_OBJECT

public:
    explicit DealDamageDialog(RPGDB *db, QJsonArray parameters, QWidget *parent = nullptr);
    ~DealDamageDialog();

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
    Ui::DealDamageDialog *ui;
};

#endif // DEALDAMAGEDIALOG_H
