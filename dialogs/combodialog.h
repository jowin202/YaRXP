#ifndef COMBODIALOG_H
#define COMBODIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QComboBox>


namespace Ui {
class ComboDialog;
}

class ComboDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ComboDialog(QWidget *parent = nullptr);
    ~ComboDialog();

    void keyPressEvent(QKeyEvent *event)
    {
        if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
        else if (event->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
    }


    void setText(QString win_title, QString text);

    QComboBox *combo();


signals:
    void ok_clicked(int);

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

private:
    Ui::ComboDialog *ui;
};

#endif // COMBODIALOG_H
