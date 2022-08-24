#ifndef EDITCLASSES_H
#define EDITCLASSES_H

#include <QWidget>

namespace Ui {
class EditClasses;
}

class EditClasses : public QWidget
{
    Q_OBJECT

public:
    explicit EditClasses(QWidget *parent = nullptr);
    ~EditClasses();

private:
    Ui::EditClasses *ui;
};

#endif // EDITCLASSES_H
