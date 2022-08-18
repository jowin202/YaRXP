#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H

#include <QWidget>
#include <QDir>
#include <QDebug>
#include <QDirIterator>


#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgaudiofile.h"

namespace Ui {
class AudioWidget;
}

class AudioWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudioWidget(QWidget *parent = nullptr);
    ~AudioWidget();
    void setSystem(RPGSystem *system)
    { this->system = system; }

    void save_data_to_object();



    enum {BGM, BGS, ME, SE};
    void setData(int mode, RPGAudioFile *audiofile);

private:
    Ui::AudioWidget *ui;

    RPGSystem *system;
    int mode = -1;
    RPGAudioFile *audiofile = 0;
};

#endif // AUDIOWIDGET_H
