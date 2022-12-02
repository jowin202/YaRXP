#ifndef SKILLLEARNING_H
#define SKILLLEARNING_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class SkillLearning;
}

class RPGEditorController;

class SkillLearning : public QWidget
{
    Q_OBJECT

public:
    explicit SkillLearning(RPGEditorController *ec, int row, int level, int skill, QWidget *parent = nullptr);
    ~SkillLearning();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


signals:
    void ok_clicked(int,int,int);

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

private:
    Ui::SkillLearning *ui;
    RPGEditorController *ec;

    int row;
};

#endif // SKILLLEARNING_H
