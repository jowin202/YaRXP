#ifndef OBJECTCOMBODIALOG_H
#define OBJECTCOMBODIALOG_H

#include <QWidget>
#include <QKeyEvent>

class RPGSystem;

namespace Ui {
class ObjectComboDialog;
}

class ObjectComboDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ObjectComboDialog(QWidget *parent = nullptr);
    ~ObjectComboDialog();

    void keyPressEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
        else if (event->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
    }


    void setText(QString text);
    void fillCombo(RPGSystem *system, int type, bool shownum, int chars, int current, bool allow_none);


signals:
    void ok_clicked(int);

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

private:
    Ui::ObjectComboDialog *ui;
};

#endif // OBJECTCOMBODIALOG_H
