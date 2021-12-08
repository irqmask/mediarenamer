#include <QDateTime>
#include <QDebug>
#include <QDirIterator>
#include <QFileDialog>
#include <QProcess>
#include <QRegExp>
#include <QStringList>

#include <sys/types.h>
#include <utime.h>

#include "fileextensionfilter.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
                                        , ui(new Ui::MainWindow)
                                        , m_folder("")
                                        , m_fileListModel(this, &m_files)
{
    ui->setupUi(this);
    loadSettings();
    ui->tblFileList->setModel(&m_fileListModel);
    ui->tblFileList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->btnPreview->setEnabled(false);
    ui->btnRename->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    saveSettings();
}

void MainWindow::loadSettings()
{
    ui->txtFolder->setText(m_settings.value("lastFolder", "").toString());
    ui->txtTitle->setText(m_settings.value("lastTitle", "").toString());
    ui->cbFileUTCTimestamp->setChecked(m_settings.value("filesHaveUTCTimestamp", true).toBool());
}

void MainWindow::saveSettings()
{
    m_settings.setValue("lastFolder", ui->txtFolder->text());
    m_settings.setValue("lastTitle", ui->txtTitle->text());
    m_settings.setValue("filesHaveUTCTimestamp", ui->cbFileUTCTimestamp->isChecked());
}

void MainWindow::on_btnSelectFolder_clicked()
{
    QString lastFolderAsString = ui->txtFolder->text();
    QString folder = QFileDialog::getExistingDirectory(0, ("Bitte Medienverzeichnis wählen"), lastFolderAsString);
    ui->txtFolder->setText(folder);

    m_folder = folder;

    loadFileList();

    ui->btnPreview->setEnabled(true);
    ui->btnRename->setEnabled(false);
    ui->tblFileList->resizeColumnsToContents();
}

void MainWindow::on_btnPreview_clicked()
{
    createPreview();

    m_fileListModel.fileListChanged();
    ui->tblFileList->resizeColumnsToContents();
    ui->btnRename->setEnabled(true);
}

void MainWindow::on_btnRename_clicked()
{
    renameFiles();

    loadFileList();
    ui->btnPreview->setEnabled(true);
    ui->btnRename->setEnabled(false);
    ui->tblFileList->resizeColumnsToContents();
}

int MainWindow::countFiles()
{
    int count = 0;

    QDirIterator it(m_folder);
    while (it.hasNext()) {
        if (!it.fileInfo().isFile()) {
            it.next();
            continue;
        }
        if (m_filter.getType(it.fileName()) != Unknown) {
            count++;
        }
        it.next();
    }
    return count;
}

QDateTime MainWindow::getDateFromFilename(QString filename)
{
    QString fn;
    if (filename.contains("IMG_"))
        fn = filename.right(filename.length() - 4);
    else if (filename.contains("VID_"))
        fn = filename.right(filename.length() - 4);
    else
        fn = filename;

    QDateTime dt;
    QString dateformat;
    if (fn.contains("-"))
        dateformat = "yyyy-MM-dd_hhmmss";
    else
        dateformat = "yyyyMMdd_hhmmss";

    fn = fn.left(dateformat.length());
    dt = QDateTime::fromString(fn, dateformat);

    return dt;
}

bool MainWindow::getImageCreationDate(QString filename, QDateTime *datetime)
{
    // dirty hack.
    // files beginning with IMG are from canon camera's SD-card which is interpreted by Ubuntu as UTC, which is wrong
    if (filename.contains("IMG_")) {
        *datetime = datetime->toUTC();
    }
    return true;
}

bool MainWindow::getVideoCreationDate(QString filename, QDateTime *datetime)
{
    QProcess process;
    process.start(QString("ffmpeg -i %1").arg(filename));
    process.waitForFinished(-1); // will wait forever until finished

    QString stderr = process.readAllStandardError();
    QStringList lines = stderr.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    for (QString& line : lines) {
        if (!line.contains("creation_time")) continue;
        qDebug() << line;
        int begin = line.indexOf(QRegExp("[0-9]"));
        int end = line.indexOf(".");
        line = line.mid(begin, end - begin) + "Z";
        if (begin < 0 || end < 0 || begin >= end) return false;
        QDateTime dt;
        dt = dt.fromString(line, Qt::ISODate);
        dt = dt.toLocalTime();
        *datetime = dt;
        return true;
    }
    return false;
}

void MainWindow::setFileTime(QString filename, QDateTime datetime)
{
    utimbuf tb;
    tb.modtime = datetime.toTime_t(); // QDateTime::currentDateTime().toTime_t();
    tb.actime = datetime.toTime_t();

    if (utime(filename.toLocal8Bit(), &tb) != 0) {
        qInfo() << "ERROR unable to set filetime " << filename << " error " << strerror(errno);
    }
}

void MainWindow::loadFileList()
{
    m_fileListModel.beginRemoveFiles();
    m_files.clear();
    m_fileListModel.endRemoveFiles();

    int count = countFiles();

    m_fileListModel.beginInsertFiles(count);
    QDirIterator it(m_folder);
    while (it.hasNext()) {
        if (!it.fileInfo().isFile()) {
            it.next();
            continue;
        }
        FileType filetype = m_filter.getType(it.fileName());
        if (filetype != Unknown) {
            FileData fd;

            fd.setNameOld(it.fileName());
            fd.setPathAndName(it.filePath());
            fd.setFileExtension(m_filter.getExtension(it.fileName()));

            QDateTime dt;
            dt = it.fileInfo().lastModified();
            fd.setDateTime(dt);

            fd.setFilenameDateTime(getDateFromFilename(it.fileName()));

            if (filetype == Image && getImageCreationDate(it.filePath(), &dt)) {
                fd.setMediaDateTime(dt);
            }
            if (filetype == Video && getVideoCreationDate(it.filePath(), &dt)) {
                fd.setMediaDateTime(dt);
            }
            qDebug() << fd.getNameOld() << "   " << it.fileInfo().lastModified().toString() << "   " << dt.toString();
            m_files.push_back(fd);
        }
        it.next();
    }
    m_fileListModel.endInsertFiles();

    qSort(m_files.begin(), m_files.end(),
          [](const FileData& a, const FileData& b) -> bool { return a.getNameOld() < b.getNameOld(); });
    m_fileListModel.fileListChanged();
}

bool MainWindow::listContainsEntryWithNewName(QString &newName)
{
    for (FileData &fd : m_files) {
        if (fd.getNameNew() == newName) return true;
    }
    return false;
}

void MainWindow::createPreview()
{
    QString title = ui->txtTitle->text();
    // replace spaces with _
    for (int idx=0; idx<title.length(); idx++) {
        if (title[idx] == ' ') {
            title[idx] = '_';
        }
    }

    for (FileData& fd : m_files) {
        // create new_name
        QDateTime dt;

        if (ui->cbUseFilenameDate->isChecked()) {
            dt = fd.getFilenameDateTime();
        } else if (fd.isMediaDateTimeSet()) {
            dt = fd.getMediaDateTime();
        } else {
            dt = fd.getDateTime();
            if (ui->cbFileUTCTimestamp->isChecked()) {
                dt = dt.toUTC();
            }
        }
        QString new_name = dt.toString("yyyy-MM-dd_HHmmss");
        new_name += "_";
        new_name += title;
        // append old name to new name if new name already exists.
        QString candidatename = new_name + fd.getFileExtension();
        if (listContainsEntryWithNewName(candidatename)) {
            new_name += "_";
            new_name += fd.getNameOld();
        }
        new_name += fd.getFileExtension();
        fd.setNameNew(new_name);
    }
}


void MainWindow::renameFiles()
{
    for (FileData& fd : m_files) {
        QString filename_and_path_old = QDir(m_folder).filePath(fd.getNameOld());
        QString filename_and_path_new = QDir(m_folder).filePath(fd.getNameNew());
        qDebug() << filename_and_path_old + "   " + filename_and_path_new;

        QDir file;
        file.rename(filename_and_path_old, filename_and_path_new);

        if (ui->cbRepairFileDate->isChecked())
        {
            if (fd.getFilenameDateTime().isValid()) {
                qInfo() << "repair filedate of " << filename_and_path_new;
                setFileTime(filename_and_path_new, fd.getFilenameDateTime());
            } else {
                qInfo() << "date time from filename not set";
            }
        }
    }
}
