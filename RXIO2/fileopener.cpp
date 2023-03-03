#include "fileopener.h"

FileOpener::FileOpener(QString path, QString name)
{
    this->path = path;
    this->name = name;
}

QString FileOpener::get_existing_directory()
{
    QDir dir(path);
    QFileInfoList files = dir.entryInfoList(QStringList(name), QDir::NoDotAndDotDot | QDir::Dirs);


    if (files.length() == 0) return QString();

    QString fullpath = files.at(0).absoluteFilePath();
    if (!fullpath.endsWith(QDir::separator()))
        fullpath += QDir::separator();

    return fullpath;
}

QImage FileOpener::get_image()
{
    QDir dir(path);
    QStringList filters = { name, name + ".jpg", name + ".png" };
    QFileInfoList files = dir.entryInfoList(filters, QDir::NoDotAndDotDot | QDir::Files);


    if (files.length() == 0) return QImage();

    QString fullpath = files.at(0).absoluteFilePath();

    return QImage(fullpath);
}

QString FileOpener::get_image_path()
{
    QDir dir(path);
    QStringList filters = { name, name + ".jpg", name + ".png" };
    QFileInfoList files = dir.entryInfoList(filters, QDir::NoDotAndDotDot | QDir::Files);


    if (files.length() == 0) return "";

    QString fullpath = files.at(0).absoluteFilePath();

    return fullpath;
}

QString FileOpener::get_audio()
{
    QDir dir(path);
    QStringList filters = { name, name + ".wav", name + ".mp3", name + ".ogg", name + ".mid" };
    QFileInfoList files = dir.entryInfoList(filters, QDir::NoDotAndDotDot | QDir::Files);


    if (files.length() == 0) return "";

    QString fullpath = files.at(0).absoluteFilePath();

    return fullpath;
}

QString FileOpener::get_existing_file()
{
    QDir dir(path);
    QFileInfoList files = dir.entryInfoList(QStringList(name), QDir::NoDotAndDotDot | QDir::Files);


    if (files.length() == 0) return QString();

    QString fullpath = files.at(0).absoluteFilePath();

    return fullpath;
}
