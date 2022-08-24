#ifndef EDITORACTORPARAMETERPAGE_H
#define EDITORACTORPARAMETERPAGE_H

#include <QWidget>

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

    void update_curve(int level, int value);

private:
    Ui::EditorActorParameterPage *ui;
    EditActors *parent;
    int *values;
    int param;
};

#endif // EDITORACTORPARAMETERPAGE_H
