#ifndef CONTROLVARIABLESDIALOG_H
#define CONTROLVARIABLESDIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QKeyEvent>

class RPGDB;
class RPGMapController;

namespace Ui {
class ControlVariablesDialog;
}

class ControlVariablesDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ControlVariablesDialog(RPGDB *db, RPGMapController *mc, QJsonArray parameters, QWidget *parent = nullptr);
    ~ControlVariablesDialog();

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
    Ui::ControlVariablesDialog *ui;
};

#endif // CONTROLVARIABLESDIALOG_H
