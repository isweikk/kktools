#include "pub_util.h"

#include <QFileInfo>

bool PubUtil::isFileExist(QString fullFileName)
{
    QFileInfo fileInfo(fullFileName);
    if (fileInfo.isFile()) {
        return true;
    }
    return false;
}

bool PubUtil::isDirExist(QString fullPath)
{
    QFileInfo fileInfo(fullPath);
    if (fileInfo.isDir()) {
        return true;
    }
    return false;
}
