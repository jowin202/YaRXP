#ifndef EDITSKILLS_H
#define EDITSKILLS_H

#include <QWidget>

class RPGEditorController;

namespace Ui {
class EditSkills;
}

class EditSkills : public QWidget
{
    Q_OBJECT

public:
    explicit EditSkills(QWidget *parent = nullptr);
    ~EditSkills();


    void setEC(RPGEditorController *ec);

    void set_skill(int n);
    void save();


private slots:
    void on_button_se_clicked();

    void on_button_icon_clicked();

private:
    Ui::EditSkills *ui;
    RPGEditorController *ec;
    int current;

    //RPGAudioFile menu_se;
};

#endif // EDITSKILLS_H
