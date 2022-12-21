#ifndef EDITCLASSES_H
#define EDITCLASSES_H

#include <QWidget>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <QAction>
#include <QMenu>
#include <QSettings>

class RPGEditorController;

namespace Ui {
class EditClasses;
}

class EditClasses : public QWidget
{
    Q_OBJECT

public:
    explicit EditClasses(QWidget *parent = nullptr);
    ~EditClasses();

    void setEC(RPGEditorController *ec);



public slots:

private slots:
    void on_table_skills_itemDoubleClicked(QTableWidgetItem *item);
    void on_button_skill_add_clicked();
    void on_button_skill_del_clicked();



private:
    Ui::EditClasses *ui;
    RPGEditorController *ec = 0;

};

#endif // EDITCLASSES_H
