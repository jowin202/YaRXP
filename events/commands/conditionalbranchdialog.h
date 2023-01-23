#ifndef CONDITIONALBRANCHDIALOG_H
#define CONDITIONALBRANCHDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonArray>
#include <QButtonGroup>

class RPGDB;
class RPGMapController;

namespace Ui {
class ConditionalBranchDialog;
}

class ConditionalBranchDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ConditionalBranchDialog(RPGDB *db, RPGMapController *mc, QJsonArray parameters, QWidget *parent = nullptr);
    ~ConditionalBranchDialog();
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
    Ui::ConditionalBranchDialog *ui;
    RPGDB *db;
    RPGMapController *mc;
    QButtonGroup maingroup;
    QButtonGroup variablegroup;
    QButtonGroup actorgroup;
    QButtonGroup enemygroup;

};

#endif // CONDITIONALBRANCHDIALOG_H
