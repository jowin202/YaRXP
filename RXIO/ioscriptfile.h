#ifndef IOSCRIPTFILE_H
#define IOSCRIPTFILE_H

#include <QObject>
#include <QDir>

#include "fileparser.h"

class RPGSystem;

class IOScriptFile : public FileParser
{
    Q_OBJECT
public:
    IOScriptFile();

    void read_file_to_script_folder(RPGSystem *system);
    QStringList read_file_names_from_scripts(RPGSystem *system);

    void write_script_file(RPGSystem *system, QStringList list);

};

#endif // IOSCRIPTFILE_H
