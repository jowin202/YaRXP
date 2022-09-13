#ifndef EDITCLASSES_H
#define EDITCLASSES_H

#include <QWidget>
#include <QCheckBox>
#include <QTableWidgetItem>

class RPGSystem;

namespace Ui {
class EditClasses;
}

class EditClasses : public QWidget
{
    Q_OBJECT

public:
    explicit EditClasses(QWidget *parent = nullptr);
    ~EditClasses();

    void setSystem(RPGSystem *system) {this->system = system;}

    void set_readonly(QTableWidgetItem *item)
    { item->setFlags(item->flags() & ~Qt::ItemIsEditable); }


public slots:
    void set_class(int n);
    void set_skill_from_dialog(int row, int level, int skill);

private slots:
    void on_table_skills_itemDoubleClicked(QTableWidgetItem *item);


    void on_button_skill_add_clicked();

    void on_button_skill_del_clicked();

private:
    Ui::EditClasses *ui;
    RPGSystem *system = 0;
};

#endif // EDITCLASSES_H
