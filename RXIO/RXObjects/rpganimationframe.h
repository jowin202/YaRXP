#ifndef RPGANIMATIONFRAME_H
#define RPGANIMATIONFRAME_H

#include <QObject>

class RPGAnimationFrame : public QObject
{
    Q_OBJECT
public:
    explicit RPGAnimationFrame(QObject *parent = nullptr);

    QList<int> cell_data;
    int cell_max;

    QStringList param_order;
signals:

};

#endif // RPGANIMATIONFRAME_H
