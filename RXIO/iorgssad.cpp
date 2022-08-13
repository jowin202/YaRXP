#include "iorgssad.h"

IORGSSAD::IORGSSAD(QObject *parent) : FileParser(parent)
{

}

IORGSSAD::IORGSSAD(QString path, QString output_dir)
{
    if (!output_dir.endsWith(QDir::separator()))
        output_dir.append(QDir::separator());

    this->file.setFileName(path);
    file.open(QIODevice::ReadOnly);

    QByteArray header = file.read(8);
    if (header != QByteArray::fromHex("5247535341440001"))
        throw getException("Invalid header");

    while (true)
    {
        int name_length = this->read_and_decrypt_integer();
        QString name = this->read_and_decrypt_string(name_length);
        name.replace("\\", QDir::separator()); // operating system independent
        int file_size = this->read_and_decrypt_integer();

        QByteArray buffer = this->file.read(file_size);

        qDebug() << name;


        int tempKey = key;
        QByteArray key_array;
        key_array.resize(4);
        key_array[0] = ((tempKey >>  0) & 0xFF);
        key_array[1] = ((tempKey >>  8) & 0xFF);
        key_array[2] = ((tempKey >> 16) & 0xFF);
        key_array[3] = ((tempKey >> 24) & 0xFF);




        int j = 0;
        for (int i = 0; i <= buffer.length()-1; i++)
        {
            if (j == 4)
            {
                j = 0;
                tempKey *= 7;
                tempKey += 3;

                key_array[0] = ((tempKey >>  0) & 0xFF);
                key_array[1] = ((tempKey >>  8) & 0xFF);
                key_array[2] = ((tempKey >> 16) & 0xFF);
                key_array[3] = ((tempKey >> 24) & 0xFF);

            }

            buffer[i] = 0xFF & (buffer[i] ^ key_array[j]);

            j++;
        }

        QFile ofile(output_dir + name);
        ofile.open(QIODevice::WriteOnly);
        ofile.write(buffer);
        ofile.close();




        if (this->file.atEnd())
            break;
    }



    file.close();
}

int IORGSSAD::read_and_decrypt_integer()
{
    int res = 0;
    res |= (this->read_one_byte() & 0xFF) ^ (key & 0xFF);
    res |= ((this->read_one_byte() & 0xFF) ^ ((key >>  8) & 0xFF) ) << 8;
    res |= ((this->read_one_byte() & 0xFF) ^ ((key >> 16) & 0xFF) ) << 16;
    res |= ((this->read_one_byte() & 0xFF) ^ ((key >> 24) & 0xFF) ) << 24;

    key *= 7;
    key += 3;

    return res;
}

QString IORGSSAD::read_and_decrypt_string(int length)
{
    QByteArray array = this->file.read(length);

    for (int i = 0; i <= array.length() - 1; i++)
    {
        array[i] = 0xFF & (array[i] ^ (key & 0xFF));
        key *= 7;
        key += 3;
    }

    return QString(array);
}
