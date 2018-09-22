#ifndef FILELIST_H
#define FILELIST_H

#include <QList>
#include "filedata.h"

class FileList : public QList<FileData>
{
public:
    FileList();
};

#endif // FILELIST_H
