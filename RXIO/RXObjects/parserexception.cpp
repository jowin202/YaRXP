#include "parserexception.h"

ParserException::ParserException(QString message, QVariantList error_data)
{
    this->message = message;
    this->error_data = error_data;
}
