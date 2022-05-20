#include "audiowidget.h"
#include "ui_audiowidget.h"


AudioWidget::AudioWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioWidget)
{
    ui->setupUi(this);
}

AudioWidget::~AudioWidget()
{
    delete ui;
}

void AudioWidget::save_data_to_object()
{
    if (this->audiofile != 0)
    {
        if (this->ui->listWidget->selectedItems().size() == 1)
            this->audiofile->name = this->ui->listWidget->currentItem()->text().chopped(4);
        this->audiofile->pitch = this->ui->verticalSlider_pitch->value();
        this->audiofile->volume = this->ui->verticalSlider_volume->value();

    }
}


void AudioWidget::setData(int mode, RPGAudioFile *audiofile)
{
    this->audiofile = audiofile;
    this->ui->verticalSlider_volume->setValue(this->audiofile->volume);
    this->ui->verticalSlider_pitch->setValue(this->audiofile->pitch);


    this->mode = mode;
    QString music_type;

    switch(mode)
    {
    case BGM: music_type = "BGM"; break;
    case BGS: music_type = "BGS"; break;
    case ME: music_type = "ME"; break;
    case SE: music_type = "SE"; break;
    }

    QDir music_dir(this->settings->current_project_dir + QDir::separator() + "Audio" + QDir::separator() + music_type);

    QString name = audiofile->name;


    QDirIterator it(music_dir);
    while (it.hasNext()) {
        it.next();
        if (it.fileName() == "." || it.fileName() == "..")
            continue;


        this->ui->listWidget->addItem(it.fileName());
        if (this->audiofile->name == it.fileName().chopped(4))
        {
            this->ui->listWidget->setCurrentItem(this->ui->listWidget->item(this->ui->listWidget->count()-1));
        }
    }


}
