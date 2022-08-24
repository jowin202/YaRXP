#ifndef EDITEXPCURVE_H
#define EDITEXPCURVE_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class EditExpCurve;
}

class EditExpCurve : public QWidget
{
    Q_OBJECT

public:
    explicit EditExpCurve(QWidget *parent = nullptr);
    ~EditExpCurve();

    void set_values(int basis, int inflation);


    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->close();
    }

public slots:
    void update_levels();

private slots:
    void on_button_ok_clicked();
    void on_button_cancel_clicked();

signals:
    void exp_curve_set(int,int);

private:
    Ui::EditExpCurve *ui;
};

#endif // EDITEXPCURVE_H
