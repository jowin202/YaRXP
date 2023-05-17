#ifndef PBSEDITOR_H
#define PBSEDITOR_H

#include <QWidget>

namespace Ui {
class PBSEditor;
}

class PBSEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PBSEditor(QWidget *parent = nullptr);
    ~PBSEditor();

private:
    Ui::PBSEditor *ui;
};

#endif // PBSEDITOR_H
