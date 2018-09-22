#include <QDebug>
#include <QString>

#include "fileextensionfilter.h"

TypeFilter::TypeFilter(FileType type, QString extension) : m_type(type)
                                                         , m_extension(extension)
{
}

FileType TypeFilter::type() const
{
    return m_type;
}

QString TypeFilter::extension() const
{
    return m_extension;
}

// ---

FileExtensionFilter::FileExtensionFilter(QObject *parent) : QObject(parent)
{
    m_filterList.push_back(TypeFilter(Image, QString(".JPG")));
    m_filterList.push_back(TypeFilter(Image, QString(".JPEG")));
    m_filterList.push_back(TypeFilter(Video, QString(".MOV")));
    m_filterList.push_back(TypeFilter(Video, QString(".AVI")));
    m_filterList.push_back(TypeFilter(Video, QString(".MPG")));
    m_filterList.push_back(TypeFilter(Video, QString(".MPEG")));
    m_filterList.push_back(TypeFilter(Video, QString(".MP4")));
}

FileType FileExtensionFilter::getType(QString filename)
{
    int last_dot_index = filename.lastIndexOf(".");
    int remaining = filename.length() - last_dot_index;

    if (last_dot_index > 0 && remaining > 0) {
        QString extension = filename.right(remaining);

        extension = extension.toUpper();

        for (TypeFilter& tf : m_filterList) {
            if (tf.extension() == extension) {
                return tf.type();
            }
        }
    }
    return Unknown;
}

QString FileExtensionFilter::getExtension(QString filename)
{
    QString extension = "";
    int last_dot_index = filename.lastIndexOf(".");
    int remaining = filename.length() - last_dot_index;

    if (last_dot_index > 0 && remaining > 0) {
        extension = filename.right(remaining);
    }
    return extension;
}


