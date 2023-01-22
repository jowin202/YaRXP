#ifndef COMBOCOMBODIALOG_H
#define COMBOCOMBODIALOG_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QKeyEvent>

class RPGDB;
class RPGMapController;

namespace Ui {
class ComboComboDialog;
}

class ComboComboDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ComboComboDialog(RPGDB *db, RPGMapController *mc, int code, QJsonArray parameters, QWidget *parent = nullptr);
    ~ComboComboDialog();

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
    Ui::ComboComboDialog *ui;
    int code;
};

#endif // COMBOCOMBODIALOG_H
