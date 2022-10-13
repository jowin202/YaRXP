#ifndef EDITORACTORPARAMETERS_H
#define EDITORACTORPARAMETERS_H

#include <QWidget>
#include <QKeyEvent>
#include <QDebug>

namespace Ui {
class EditorActorParameters;
}

class EditActors;

class EditorActorParameters : public QWidget
{
    Q_OBJECT

public:
    explicit EditorActorParameters(QWidget *parent = nullptr);
    ~EditorActorParameters();

    void set_parent(EditActors *parent);
    void set_page(int page);


    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if(e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


signals:
    void params_set();

private slots:
    void on_button_cancel_clicked();

    void on_button_ok_clicked();

private:
    Ui::EditorActorParameters *ui;
    int maxhp[100];
    int maxsp[100];
    int str[100];
    int dex[100];
    int agi[100];
    int int_var[100];

    EditActors *parent;

};

#endif // EDITORACTORPARAMETERS_H
