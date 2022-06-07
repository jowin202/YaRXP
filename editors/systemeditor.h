#ifndef SYSTEMEDITOR_H
#define SYSTEMEDITOR_H

#include <QWidget>

namespace Ui {
class SystemEditor;
}

class SystemEditor : public QWidget
{
    Q_OBJECT

public:
    explicit SystemEditor(QWidget *parent = nullptr);
    ~SystemEditor();

private:
    Ui::SystemEditor *ui;
};

#endif // SYSTEMEDITOR_H
