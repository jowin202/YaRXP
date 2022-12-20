#ifndef EDITANIMATIONS_H
#define EDITANIMATIONS_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QTableWidgetItem>
#include <QInputDialog>

namespace Ui {
class EditAnimations;
}

class RPGDB;
class RPGEditorController;

class EditAnimations : public QWidget
{
    Q_OBJECT

public:
    explicit EditAnimations(QWidget *parent = nullptr);
    ~EditAnimations();

    void setEC(RPGEditorController *ec);

    void set_animation(int n);


    void set_readonly(QTableWidgetItem *item)
    { item->setFlags(item->flags() & ~Qt::ItemIsEditable); }


private slots:
    void on_button_animation_clicked();
    void on_button_set_frames_clicked();
    void update_frame_list();
    void update_timings();

    void on_table_timings_itemDoubleClicked(QTableWidgetItem *item);

    void add_timing();
    void edit_timing();
    void copy_timing();
    void paste_timing();
    void delete_timing();



    void on_table_timings_customContextMenuRequested(const QPoint &pos);

private:
    Ui::EditAnimations *ui;
    RPGEditorController *ec;


    QAction *action_add;
    QAction *action_edit;
    QAction *action_delete;
    QAction *action_copy;
    QAction *action_paste;

};

#endif // EDITANIMATIONS_H
