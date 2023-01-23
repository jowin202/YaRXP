#ifndef MOVEROUTEDIALOG_H
#define MOVEROUTEDIALOG_H

#include <QWidget>

namespace Ui {
class MoveRouteDialog;
}

class MoveRouteDialog : public QWidget
{
    Q_OBJECT

public:
    explicit MoveRouteDialog(QWidget *parent = nullptr);
    ~MoveRouteDialog();

private:
    Ui::MoveRouteDialog *ui;
};

#endif // MOVEROUTEDIALOG_H
