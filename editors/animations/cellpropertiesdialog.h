#ifndef CELLPROPERTIESDIALOG_H
#define CELLPROPERTIESDIALOG_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonArray>

namespace Ui {
class CellPropertiesDialog;
}

class CellPropertiesDialog : public QWidget
{
    Q_OBJECT

public:
    explicit CellPropertiesDialog(QJsonArray values, int number, int max, int max_pattern, QWidget *parent = nullptr);
    ~CellPropertiesDialog();
    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        else if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void ok_clicked(QJsonArray);
private slots:
    void on_button_ok_clicked();

    void on_button_cancel_clicked();

private:
    Ui::CellPropertiesDialog *ui;
    int max;
    int number;
    QJsonArray values;
};

#endif // CELLPROPERTIESDIALOG_H
