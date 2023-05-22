#ifndef PBSEDITOR_H
#define PBSEDITOR_H

#include <QWidget>


class RPGDB;

namespace Ui {
class PBSEditor;
}

class PBSEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PBSEditor(RPGDB *db, QWidget *parent = nullptr);
    ~PBSEditor();

private slots:
    void on_button_mapConnection_clicked();

    void on_button_mapencounter_clicked();

private:
    Ui::PBSEditor *ui;
    RPGDB *db;
};

#endif // PBSEDITOR_H
