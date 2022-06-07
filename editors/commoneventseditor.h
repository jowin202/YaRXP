#ifndef COMMONEVENTSEDITOR_H
#define COMMONEVENTSEDITOR_H

#include <QWidget>

namespace Ui {
class CommonEventsEditor;
}

class CommonEventsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CommonEventsEditor(QWidget *parent = nullptr);
    ~CommonEventsEditor();

private:
    Ui::CommonEventsEditor *ui;
};

#endif // COMMONEVENTSEDITOR_H
