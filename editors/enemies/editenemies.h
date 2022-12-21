#ifndef EDITENEMIES_H
#define EDITENEMIES_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QAction>
#include <QMenu>
#include <QSettings>


namespace Ui {
class EditEnemies;
}

class RPGEditorController;

class EditEnemies : public QWidget
{
    Q_OBJECT

public:
    explicit EditEnemies(QWidget *parent = nullptr);
    ~EditEnemies();



    void setEC(RPGEditorController *ec);



public slots:



private slots:
    void on_table_action_itemDoubleClicked(QTableWidgetItem *item);
    void on_button_action_del_clicked();
    void on_button_action_add_clicked();


private:
    Ui::EditEnemies *ui;
    RPGEditorController *ec;


};

#endif // EDITENEMIES_H
