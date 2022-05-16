#ifndef MAPPROPERTIES_H
#define MAPPROPERTIES_H

#include <QWidget>

namespace Ui {
class MapProperties;
}

class MapProperties : public QWidget
{
    Q_OBJECT

public:
    explicit MapProperties(QWidget *parent = nullptr);
    ~MapProperties();

private:
    Ui::MapProperties *ui;
};

#endif // MAPPROPERTIES_H
