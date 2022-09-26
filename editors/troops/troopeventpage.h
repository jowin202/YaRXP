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
    void update_condition();

private:
    Ui::TroopEventPage *ui;
    RPGSystem *system;
    RPGTroopPage *page;
    TroopPicLabel *troop_pic_label;

    //condition
    bool enemy_valid;
    bool switch_valid;
    bool turn_valid;
    bool actor_valid;
    int switch_id;
    int turn_a;
    int turn_b;
    int enemy_index;
    int enemy_hp;
    int actor_hp;
    int actor_id;


};

#endif // TROOPEVENTPAGE_H
