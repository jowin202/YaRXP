#ifndef RPGEVENTPAGECONDITION_H
#define RPGEVENTPAGECONDITION_H

#include <QObject>

class RPGEventPageCondition : public QObject
{
    Q_OBJECT
public:
    explicit RPGEventPageCondition(QObject *parent = nullptr);

    void setParameter(QString param, int val);
    void setParameter(QString param, bool val);


    int switch1_id;
    int switch2_id;
    int variable_id;

    bool self_switch_valid;
    bool variable_valid;
    bool switch1_valid;
    bool switch2_valid;

    int variable_value;
    int self_switch_ch; //A=1, B=2, C=3, D=4


signals:

private:



};

#endif // RPGEVENTPAGECONDITION_H
