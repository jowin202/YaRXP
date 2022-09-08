#ifndef SKILLLEARNING_H
#define SKILLLEARNING_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class SkillLearning;
}

class RPGSystem;

class SkillLearning : public QWidget
{
    Q_OBJECT

public:
    explicit SkillLearning(RPGSystem *system, int row, int level, int skill, QWidget *parent = nullptr);
    ~SkillLearning();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if (e->key() == Qt::Key_Return)
            this->on_button_ok_clicked();
    }


signals:
    void ok_clicked(int,int,int);

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

private:
    Ui::SkillLearning *ui;
    RPGSystem *system;

    int row;
};

#endif // SKILLLEARNING_H
