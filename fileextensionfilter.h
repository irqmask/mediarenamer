#ifndef FILEEXTENSIONFILTER_H
#define FILEEXTENSIONFILTER_H

#include <QList>
#include <QObject>

enum FileType {
    Unknown, Image, Video
};

class TypeFilter
{
public:
    TypeFilter(FileType type, QString extension);

    FileType type() const;

    QString extension() const;

private:
    FileType m_type;
    QString m_extension;
};

class FileExtensionFilter : public QObject
{
    Q_OBJECT
public:
    explicit FileExtensionFilter(QObject *parent = 0);

    FileType getType(QString filename);
    QString getExtension(QString filename);

signals:

public slots:

private:
    QList<TypeFilter> m_filterList;
};

#endif // FILEEXTENSIONFILTER_H
