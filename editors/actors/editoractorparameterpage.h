#ifndef EDITORACTORPARAMETERPAGE_H
#define EDITORACTORPARAMETERPAGE_H

#include <QWidget>
#include <QRandomGenerator>
#include <QDateTime>

namespace Ui {
class EditorActorParameterPage;
}

class EditActors;


class EditorActorParameterPage : public QWidget
{
    Q_OBJECT

public:
    explicit EditorActorParameterPage(QWidget *parent = nullptr);
    ~EditorActorParameterPage();

    void set_parent_and_param(int param, int *values, EditActors *parent);

private slots:
    void on_button_A_clicked();
    void on_button_B_clicked();
    void on_button_C_clicked();
    void on_button_D_clicked();
    void on_button_E_clicked();

    void redraw();
    void update_curve(int level, int value);
    void update_curve_from_spin();

    void update_spin_value(int level);

    void set_linear(int start, int end);

    void on_button_gen_curve_clicked();

private:
    Ui::EditorActorParameterPage *ui;
    EditActors *parent;
    int *values;
    int param;
    QRandomGenerator rand;
};

#endif // EDITORACTORPARAMETERPAGE_H
