#ifndef EDITSCRIPTS_H
#define EDITSCRIPTS_H

#include <QWidget>
#include <QFileDialog>


class Highlighter;
class RPGEditorController;

namespace Ui {
class EditScripts;
}

class EditScripts : public QWidget
{
    Q_OBJECT

public:
    explicit EditScripts(QWidget *parent = nullptr);
    ~EditScripts();

    void setEC(RPGEditorController *ec);


public slots:
    void export_all_scripts();

private slots:
    void save();

private:
    Ui::EditScripts *ui;
    RPGEditorController *ec;
    Highlighter* highlighter;
};

#endif // EDITSCRIPTS_H
