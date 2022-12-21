#ifndef TIMINGTABLE_H
#define TIMINGTABLE_H

#include <QTableWidget>
#include <QObject>
#include <QWidget>

#include <QAction>
#include <QMenu>

#include <QJsonObject>
#include <QJsonArray>

class RPGDB;
class RPGEditorController;

class TimingTable : public QTableWidget
{
    Q_OBJECT
public:
    TimingTable(QWidget *parent);
    void setEC(RPGEditorController *ec) {this->ec = ec; }


    void set_readonly(QTableWidgetItem *item)
    { item->setFlags(item->flags() & ~Qt::ItemIsEditable); }

public slots:
    void update_timings();
    void add_timing();
    void edit_timing();
    void copy_timing();
    void paste_timing();
    void delete_timing();
    void on_customContextMenuRequested(const QPoint &pos);
private:
    RPGEditorController *ec;

    QAction *action_add;
    QAction *action_edit;
    QAction *action_delete;
    QAction *action_copy;
    QAction *action_paste;


};

#endif // TIMINGTABLE_H
