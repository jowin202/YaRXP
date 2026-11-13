#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <QObject>
#include <QList>
#include <QCoreApplication>
#include <QMap>
#include <QJsonObject>

class SearchThread : public QThread
{
    Q_OBJECT
public:
    //maps is a snapshot of all maps (QJsonObjects are implicitly shared, so this is cheap),
    //the thread must not access the documents in RPGDB while the GUI thread changes them
    explicit SearchThread(QMap<int,QJsonObject> maps, int mode, QString text, int sw_var, Qt::CaseSensitivity cs);

    enum SearchMode { TEXT, SWITCH, VARIABLE, COMMAND };
    void run();

    QMap<int,QJsonObject> maps;
    int mode;
    QString text;
    int sw_var;
    Qt::CaseSensitivity case_sensitive_options;

    QList<int> result;


signals:
    void search_finished(QList<int>);
};

#endif // SEARCHTHREAD_H
