#ifndef EDITCLASSES_H
#define EDITCLASSES_H

#include <QWidget>
#include <QCheckBox>
#include <QTableWidgetItem>

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

    void set_readonly(QTableWidgetItem *item)
    { item->setFlags(item->flags() & ~Qt::ItemIsEditable); }


public slots:
    void set_skill_from_dialog(int row, int level, int skill);
    void update_skill_from_file();

private slots:
    void on_table_skills_itemDoubleClicked(QTableWidgetItem *item);


    void on_button_skill_add_clicked();

    void on_button_skill_del_clicked();

private:
    Ui::EditClasses *ui;
    RPGEditorController *ec = 0;

    int current;
};

#endif // EDITCLASSES_H
