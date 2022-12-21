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



private slots:
    void on_button_animation_clicked();
    void on_button_set_frames_clicked();
    void update_frame_list();

    void on_table_timings_itemDoubleClicked(QTableWidgetItem *item);

    void on_frame_list_currentRowChanged(int currentRow);

private:
    Ui::EditAnimations *ui;
    RPGEditorController *ec;

};

#endif // EDITANIMATIONS_H
