#ifndef FILEDATA_H
#define FILEDATA_H

#include <QDateTime>
#include <QString>

class FileData
{
public:
    FileData();

    QString getNameOld() const;
    void setNameOld(const QString &value);

    QString getNameNew() const;
    void setNameNew(const QString &value);

    QString getPathAndName() const;
    void setPathAndName(const QString &value);

    QString getFileExtension() const;
    void setFileExtension(const QString &fileExtension);

    QDateTime getDateTime() const;
    void setDateTime(const QDateTime &dateTime);

    QDateTime getMediaDateTime() const;
    void setMediaDateTime(const QDateTime &dateTime);

    bool isMediaDateTimeSet() const;

    const QDateTime &getFilenameDateTime() const;
    void setFilenameDateTime(const QDateTime &newFilenameDateTime);

private:
    QString m_nameOld;
    QString m_nameNew;
    QString m_pathAndName;
    QString m_fileExtension;
    QDateTime m_dateTime;
    QDateTime m_mediaDateTime;
    QDateTime m_filenameDateTime;
    bool m_mediaDateTimeSet;
};

#endif // FILEDATA_H
