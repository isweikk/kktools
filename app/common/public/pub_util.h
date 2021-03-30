#ifndef PUBUTIL_H
#define PUBUTIL_H

#include <qstring.h>

class PubUtil
{
public:
    static bool isFileExist(QString fullFileName);
    static bool isDirExist(QString fullPath);

};

#endif // PUBUTIL_H
