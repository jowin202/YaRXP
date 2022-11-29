#ifndef EDITARMORS_H
#define EDITARMORS_H

#include <QWidget>



class RPGDB;
class RPGEditorController;

namespace Ui {
class EditArmors;
}

class EditArmors : public QWidget
{
    Q_OBJECT

public:
    explicit EditArmors(QWidget *parent = nullptr);
    ~EditArmors();

    void setEC(RPGEditorController *ec);

    void set_armor(int n);
    void save();


public slots:

private slots:
    void on_button_icon_clicked();

private:
    Ui::EditArmors *ui;
    RPGEditorController *ec;
};

#endif // EDITARMORS_H
