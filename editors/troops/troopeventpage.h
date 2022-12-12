#ifndef TROOPEVENTPAGE_H
#define TROOPEVENTPAGE_H

#include <QWidget>

class RPGEditorController;

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
    int page_num;


};

#endif // TROOPEVENTPAGE_H
