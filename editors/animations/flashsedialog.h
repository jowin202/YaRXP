#ifndef FLASHSEDIALOG_H
#define FLASHSEDIALOG_H

#include <QWidget>

namespace Ui {
class FlashSEDialog;
}

class FlashSEDialog : public QWidget
{
    Q_OBJECT

public:
    explicit FlashSEDialog(QWidget *parent = nullptr);
    ~FlashSEDialog();

private:
    Ui::FlashSEDialog *ui;
};

#endif // FLASHSEDIALOG_H
