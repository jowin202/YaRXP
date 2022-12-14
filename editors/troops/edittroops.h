#ifndef EDITTROOPS_H
#define EDITTROOPS_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class EditTroops;
}

class RPGEditorController;
class EditTroops : public QWidget
{
    Q_OBJECT

public:
    explicit EditTroops(QWidget *parent = nullptr);
    ~EditTroops();

    void setEC(RPGEditorController *ec);

    //void set_troop(int n);

private slots:
    void on_button_arrange_clicked();
    void on_button_clear_clicked();
    void on_button_remove_clicked();
    void on_button_add_clicked();


    void on_button_autoname_clicked();

    void on_button_edit_battleback_clicked();

    void set_new_battleback(QString battleback_name);

    void on_button_delete_page_clicked();
    void on_button_copy_page_clicked();
    void on_button_paste_page_clicked();

private:
    Ui::EditTroops *ui;
    RPGEditorController *ec;
};

#endif // EDITTROOPS_H
