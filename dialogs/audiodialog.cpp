#include "audiodialog.h"
#include "ui_audiodialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/fileopener.h"

AudioDialog::AudioDialog(RPGDB *db, QString name, int volume, int pitch, int mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudioDialog)
{
    ui->setupUi(this);
    player.setAudioOutput(&this->audio_out);


    this->db = db;
    this->mode = mode;

    this->name = name;

    this->ui->slider_volume->setValue(volume);
    this->ui->slider_pitch->setValue(pitch);


    switch(mode)
    {
    case BGM:
        this->setWindowTitle("BGM");
        this->directory = db->bgm_dir;
        break;
    case BGS:
        this->setWindowTitle("BGS");
        this->directory = db->bgs_dir;
        break;
    case ME:
        this->setWindowTitle("ME");
        this->directory = db->me_dir;
        break;
    case SE:
        this->setWindowTitle("SE");
        this->directory = db->se_dir;
        break;
    }


    this->update_audio_list();
}

AudioDialog::~AudioDialog()
{
    delete ui;
}

void AudioDialog::update_audio_list()
{
    this->ui->list->clear();
    this->ui->list->addItem("(None)");
    QDir music_dir(this->directory);


    QStringList entries = music_dir.entryList(QDir::Filter::Files | QDir::Filter::NoDotAndDotDot, QDir::SortFlag::Name);
    for (QString entry : entries)
    {
        this->ui->list->addItem(entry);
        if (this->name == entry.chopped(4))
        {
            this->ui->list->setCurrentItem(this->ui->list->item(this->ui->list->count()-1));
        }
    }
}

void AudioDialog::on_button_ok_clicked()
{
    if (this->ui->list->currentItem() == 0)
        return; // do nothing

    this->close();
    if (this->ui->list->currentItem()!= 0)
        emit ok_clicked(this->ui->list->currentItem()->text().chopped(4),this->ui->slider_volume->value(), this->ui->slider_pitch->value());
}

void AudioDialog::on_button_cancel_clicked()
{
    this->close();
}

void AudioDialog::on_button_play_clicked()
{

    connect(&player, &QMediaPlayer::errorOccurred, this, [=](QMediaPlayer::Error error, const QString &errorString){ qDebug() << error << errorString; });

    player.setSource(QUrl::fromLocalFile(FileOpener(this->directory,this->ui->list->currentItem()->text()).get_audio()));
    player.play();
}

void AudioDialog::on_button_stop_clicked()
{
    player.stop();
}

void AudioDialog::on_list_doubleClicked(const QModelIndex &index)
{
    if (index.row() >= 0)
        this->on_button_ok_clicked();
}

