#ifndef EDITSYSTEM_H
#define EDITSYSTEM_H

#include <QWidget>

namespace Ui {
class EditSystem;
}

class EditSystem : public QWidget
{
    Q_OBJECT

public:
    explicit EditSystem(QWidget *parent = nullptr);
    ~EditSystem();

private:
    Ui::EditSystem *ui;
};

#endif // EDITSYSTEM_H
