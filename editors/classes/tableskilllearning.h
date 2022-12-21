#ifndef TABLESKILLLEARNING_H
#define TABLESKILLLEARNING_H

#include <QTableWidget>
#include <QObject>
#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QTableWidget>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>


class RPGEditorController;

class TableSkillLearning : public QTableWidget
{
    Q_OBJECT
public:
    TableSkillLearning(QWidget *parent);
    void setEC(RPGEditorController *ec) {this->ec = ec;}

    void set_readonly(QTableWidgetItem *item)
    { item->setFlags(item->flags() & ~Qt::ItemIsEditable); }


public slots:
    void update_skill_from_file();
    void set_skill_from_dialog(int row, int level, int skill);
    void item_add();
    void item_delete();
    void item_copy();
    void item_paste();
    void item_edit();
    void on_customContextMenuRequested(const QPoint &pos);

private:
    QAction *action_add;
    QAction *action_edit;
    QAction *action_delete;
    QAction *action_copy;
    QAction *action_paste;

    RPGEditorController *ec;
};

#endif // TABLESKILLLEARNING_H
