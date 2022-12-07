#ifndef TROOPEVENTPAGE_H
#define TROOPEVENTPAGE_H

#include <QWidget>

class RPGEditorController;
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

    void setTroopPage(RPGEditorController *ec, int page_num);



private slots:

    void on_button_condition_clicked();
    void update_condition();

    void on_combo_span_currentIndexChanged(int index);

private:
    Ui::TroopEventPage *ui;
    RPGEditorController *ec;
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
