#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H

#include <QWidget>
#include <QDir>
#include <QDebug>
#include <QDirIterator>


#include "settings.h"
#include "parser/rpgaudiofile.h"

namespace Ui {
class AudioWidget;
}

class AudioWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudioWidget(QWidget *parent = nullptr);
    ~AudioWidget();
    void setSettings(Settings *settings)
    { this->settings = settings; }


    enum {BGM, BGS, ME, SE};
    void setData(int mode, RPGAudioFile *audiofile);

private:
    Ui::AudioWidget *ui;

    Settings *settings;
    int mode = -1;
    RPGAudioFile *audiofile = 0;
};

#endif // AUDIOWIDGET_H
