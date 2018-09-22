#ifndef FILELISTMODEL_H
#define FILELISTMODEL_H

#include <QAbstractTableModel>

#include "filelist.h"

class FileListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    FileListModel(QObject*           parent,
                  const FileList*    list);

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void beginInsertFiles(int number);
    void endInsertFiles();
    void beginRemoveFiles();
    void endRemoveFiles();
    void fileListChanged();

//signals:
//    void dataChanged(const QModelIndex&, const QModelIndex&);

    QString folder() const;
    void setFolder(const QString &folder);

private:
    QObject* m_parent;
    const FileList* m_list;
    QString m_folder;
};

#endif // FILELISTMODEL_H
