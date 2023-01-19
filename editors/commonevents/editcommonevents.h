#ifndef EDITCOMMONEVENTS_H
#define EDITCOMMONEVENTS_H

#include <QWidget>

namespace Ui {
class EditCommonEvents;
}

class RPGDB;
class RPGEditorController;
class RPGEventListController;

class EditCommonEvents : public QWidget
{
    Q_OBJECT

public:
    explicit EditCommonEvents(QWidget *parent = nullptr);
    ~EditCommonEvents();

    void setEC(RPGEditorController *ec);



private slots:

    void on_combo_trigger_currentIndexChanged(int index);

private:
    Ui::EditCommonEvents *ui;
    RPGEditorController *ec;
    RPGEventListController *ecl = 0;
};

#endif // EDITCOMMONEVENTS_H
