#ifndef GENERATECURVEDIALOG_H
#define GENERATECURVEDIALOG_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class GenerateCurveDialog;
}

class GenerateCurveDialog : public QWidget
{
    Q_OBJECT

public:
    explicit GenerateCurveDialog(int *values, int max_val, QWidget *parent = nullptr);
    ~GenerateCurveDialog();

    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }


signals:
    void ok_clicked();

private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::GenerateCurveDialog *ui;
    int *values;
};

#endif // GENERATECURVEDIALOG_H
