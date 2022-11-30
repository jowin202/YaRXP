#ifndef EDITITEMS_H
#define EDITITEMS_H

#include <QWidget>


namespace Ui {
class EditItems;
}

class RPGEditorController;

class EditItems : public QWidget
{
    Q_OBJECT

public:
    explicit EditItems(QWidget *parent = nullptr);
    ~EditItems();

    void setEC(RPGEditorController *ec);

    void set_item(int n);
    void save();


private slots:
    void on_combo_parameter_currentIndexChanged(int index);


    void on_button_se_clicked();

    void on_button_icon_clicked();

private:
    Ui::EditItems *ui;
    RPGEditorController *ec;
};

#endif // EDITITEMS_H
