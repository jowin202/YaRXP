#ifndef EDITANIMATIONS_H
#define EDITANIMATIONS_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include <QTableWidgetItem>
#include <QInputDialog>
#include <QComboBox>

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

    int custom_round(qreal n)
    {
        if (n < 0)
            return qCeil(n);
        else return qFloor(n);
    }



private slots:
    void on_button_animation_clicked();
    void on_button_set_frames_clicked();
    void update_frame_list();

    void on_table_timings_itemDoubleClicked(QTableWidgetItem *item);
    void on_frame_list_currentRowChanged(int currentRow);
    void on_button_edit_battler_clicked();
    void on_button_paste_last_clicked();
    void on_button_copy_clicked();
    void on_button_clear_frames_clicked();
    void on_button_tweening_clicked();
    void on_button_cell_batch_clicked();
    void on_button_entire_slide_clicked();

private:
    Ui::EditAnimations *ui;
    RPGEditorController *ec;

};

#endif // EDITANIMATIONS_H
