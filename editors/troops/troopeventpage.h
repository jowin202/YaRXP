#ifndef TROOPEVENTPAGE_H
#define TROOPEVENTPAGE_H

#include <QWidget>

class RPGSystem;
class RPGTroopPage;
class TroopPicLabel;

namespace Ui {
class TroopEventPage;
}

class TroopEventPage : public QWidget
{
    Q_OBJECT

public:
    explicit TroopEventPage(QWidget *parent = nullptr);
    ~TroopEventPage();

    void setTroopPage(RPGSystem *system, RPGTroopPage *page, TroopPicLabel *label);



private slots:

    void on_button_condition_clicked();

private:
    Ui::TroopEventPage *ui;
    RPGSystem *system;
    RPGTroopPage *page;
    TroopPicLabel *troop_pic_label;
};

#endif // TROOPEVENTPAGE_H
