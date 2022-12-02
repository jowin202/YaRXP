#ifndef EDITWEAPONS_H
#define EDITWEAPONS_H

#include <QWidget>

namespace Ui {
class EditWeapons;
}

class RPGEditorController;

class EditWeapons : public QWidget
{
    Q_OBJECT

public:
    explicit EditWeapons(QWidget *parent = nullptr);
    ~EditWeapons();


    void setEC(RPGEditorController *ec);

private slots:

    void on_button_icon_clicked();

private:
    Ui::EditWeapons *ui;
    RPGEditorController *ec;
    int current;

};

#endif // EDITWEAPONS_H
