#include "filelistmodel.h"

FileListModel::FileListModel(QObject*           parent,
                             const FileList*    list) : QAbstractTableModel(parent)
                                                      , m_parent(parent)
                                                      , m_list(list)
{
}

int FileListModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    int rows = m_list->count();
    return rows;
}

int FileListModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 2;
}

QVariant FileListModel::data(const QModelIndex &index, int role) const
{
    int rowidx = index.row();
    int columnidx = index.column();

    if (role == Qt::DisplayRole && rowidx < m_list->count()) {
        switch (columnidx) {
        case 0:
            return m_list->at(rowidx).getNameOld();

        case 1:
            return m_list->at(rowidx).getNameNew();

        default:
            break;
        }
    }

    return QVariant();
}

QVariant FileListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Orientation::Horizontal) return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        switch (section) {
        case 0:
            return QString("alter Name");
        case 1:
            return QString("neuer Name");
        default:
            return QString("%1").arg(section);
        }
        break;

    default:
        break;
    }
    return QVariant();
}

void FileListModel::beginInsertFiles(int numberOfFiles)
{
    beginInsertRows(QModelIndex(), m_list->count(), m_list->count() + numberOfFiles - 1);
}

void FileListModel::endInsertFiles()
{
    endInsertRows();
}

void FileListModel::beginRemoveFiles()
{
    beginRemoveRows(QModelIndex(), 0, m_list->count() - 1);
}

void FileListModel::endRemoveFiles()
{
    endRemoveRows();
}

void FileListModel::fileListChanged()
{
    QModelIndex topleft = createIndex(0, 0);
    int rows = m_list->count();
    QModelIndex bottomright = createIndex(rows-1, 2-1);
    emit dataChanged(topleft, bottomright);
}

QString FileListModel::folder() const
{
    return m_folder;
}

void FileListModel::setFolder(const QString &folder)
{
    m_folder = folder;
}
