#include "filedata.h"

FileData::FileData() : m_nameOld("")
                     , m_nameNew("")
                     , m_pathAndName("")
                     , m_mediaDateTimeSet(false)
{
}

QString FileData::getNameOld() const
{
    return m_nameOld;
}

void FileData::setNameOld(const QString &value)
{
    m_nameOld = value;
}

QString FileData::getNameNew() const
{
    return m_nameNew;
}

void FileData::setNameNew(const QString &value)
{
    m_nameNew = value;
}

QString FileData::getPathAndName() const
{
    return m_pathAndName;
}

void FileData::setPathAndName(const QString &value)
{
    m_pathAndName = value;
}

QString FileData::getFileExtension() const
{
    return m_fileExtension;
}

void FileData::setFileExtension(const QString &fileExtension)
{
    m_fileExtension = fileExtension;
}

QDateTime FileData::getDateTime() const
{
    return m_dateTime;
}

void FileData::setDateTime(const QDateTime &dateTime)
{
    m_dateTime = dateTime;
}

QDateTime FileData::getMediaDateTime() const
{
    return m_mediaDateTime;
}

void FileData::setMediaDateTime(const QDateTime &dateTime)
{
    m_mediaDateTime = dateTime;
    m_mediaDateTimeSet = true;
}

bool FileData::isMediaDateTimeSet() const
{
    return m_mediaDateTimeSet;
}


