#ifndef SHOWTEXT_H
#define SHOWTEXT_H

#include <QObject>
#include <QWidget>

#include <QListWidget>
#include <QListWidgetItem>

class ShowText : public QListWidgetItem
{
public:
    ShowText(QString text, int indent, int code);


    QString text;
    int indent;
    int code;
};

#endif // SHOWTEXT_H
