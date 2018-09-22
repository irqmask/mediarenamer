#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "filedata.h"
#include "filelist.h"
#include "filelistmodel.h"
#include "fileextensionfilter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_btnSelectFolder_clicked();

    void on_btnPreview_clicked();

    void on_btnRename_clicked();

private:
    Ui::MainWindow          *ui;
    FileExtensionFilter     m_filter;
    FileList                m_files;
    QString                 m_folder;
    FileListModel           m_fileListModel;

    QSettings               m_settings;

    void loadSettings();
    void saveSettings();

    int countFiles();
    bool getImageCreationDate(QString filename, QDateTime* datetime);
    bool getVideoCreationDate(QString filename, QDateTime* datetime);

    void loadFileList();
    bool listContainsEntryWithNewName(QString& newName);
    void createPreview();
    void renameFiles();
};

#endif // MAINWINDOW_H
