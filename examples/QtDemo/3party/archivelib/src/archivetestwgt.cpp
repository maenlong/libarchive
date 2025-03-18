#include <archive.h>
#include <archive_entry.h>

#include <QDir>
#include <QFileInfoList>
#include <QString>
#include <QDebug>

bool compressFolderToZip(const QString &folderPath, const QString &archivePath)
{
    struct archive *a;
    struct archive_entry *entry;
    struct stat st;
    char buff[8192];
    int len;
    QFile file;

    a = archive_write_new();
    archive_write_set_format_zip(a);
    archive_write_open_filename(a, archivePath.toUtf8().data());

    QDir dir(folderPath);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    foreach (QFileInfo fileInfo, fileList)
    {
        file.setFileName(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly)) return false;

        entry = archive_entry_new();
        archive_entry_set_pathname(entry, fileInfo.fileName().toUtf8().data());
        archive_entry_set_size(entry, file.size());
        archive_entry_set_filetype(entry, AE_IFREG);
        archive_entry_set_perm(entry, 0644);
        archive_write_header(a, entry);

        while ((len = file.read(buff, sizeof(buff))) > 0)
        {
            archive_write_data(a, buff, len);
        }

        archive_entry_free(entry);
        file.close();
    }

    archive_write_close(a);
    archive_write_free(a);
    return true;
}

bool decompressZipToFolder(const QString &archivePath, const QString &outputFolderPath)
{
    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_compression_all(a);
    r = archive_read_open_filename(a, archivePath.toUtf8().data(), 10240);

    if (r != ARCHIVE_OK) return false;

    QDir outputDir(outputFolderPath);
    if (!outputDir.exists())
    {
        if (!outputDir.mkpath(".")) return false;
    }

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {
        QString filePath = outputFolderPath + "/" + QString::fromUtf8(archive_entry_pathname(entry));
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) return false;

        const void *buff;
        size_t size;
        la_int64_t offset;

        while (archive_read_data_block(a, &buff, &size, &offset) == ARCHIVE_OK)
        {
            file.write((const char *)buff, size);
        }

        file.close();
    }

    archive_read_close(a);
    archive_read_free(a);
    return true;
}



bool compressFolderToTar(const QString &folderPath, const QString &tarFilePath)
{
    struct archive *a;
    struct archive_entry *entry;
    char buff[8192];
    int len;
    QFile file;

    a = archive_write_new();
    archive_write_set_format_pax_restricted(a);
    archive_write_open_filename(a, tarFilePath.toUtf8().data());

    QDir dir(folderPath);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    foreach (QFileInfo fileInfo, fileList)
    {
        file.setFileName(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly)) return false;

        entry = archive_entry_new();
        archive_entry_set_pathname(entry, fileInfo.fileName().toUtf8().data());
        archive_entry_set_size(entry, file.size());
        archive_entry_set_filetype(entry, AE_IFREG);
        archive_entry_set_perm(entry, 0644);
        archive_write_header(a, entry);

        while ((len = file.read(buff, sizeof(buff))) > 0)
        {
            archive_write_data(a, buff, len);
        }

        archive_entry_free(entry);
        file.close();
    }

    archive_write_close(a);
    archive_write_free(a);
    return true;
}

bool decompressTarToFolder(const QString &tarFilePath, const QString &outputFolderPath)
{
    struct archive *a;
    struct archive_entry *entry;
    int r;

    a = archive_read_new();
    archive_read_support_format_tar(a);
    r = archive_read_open_filename(a, tarFilePath.toUtf8().data(), 10240);

    if (r != ARCHIVE_OK) return false;

    QDir outputDir(outputFolderPath);
    if (!outputDir.exists())
    {
        if (!outputDir.mkpath(".")) return false;
    }

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
    {
        QString filePath = outputFolderPath + "/" + QString::fromUtf8(archive_entry_pathname(entry));
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) return false;

        const void *buff;
        size_t size;
        la_int64_t offset;

        while (archive_read_data_block(a, &buff, &size, &offset) == ARCHIVE_OK)
        {
            file.write((const char *)buff, size);
        }

        file.close();
    }

    archive_read_close(a);
    archive_read_free(a);
    return true;
}





#include "archivetestwgt.h"
#include "ui_archivetestwgt.h"

ArchiveTestWgt::ArchiveTestWgt(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ArchiveTestWgt)
{
    ui->setupUi(this);
}

ArchiveTestWgt::~ArchiveTestWgt()
{
    delete ui;
}



void ArchiveTestWgt::on_testBtn_1_clicked()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString folderPath = appPath + QDir::separator() + QString("Log") + QDir::separator();
    QString filePath = appPath + QDir::separator() + QString("LogZip.zip");
    bool bRet = compressFolderToZip(folderPath, filePath);

    qDebug() << "----- ----- ----- ----- ----- ----- \n"
             << __FUNCTION__
             << "\n Ret = " << bRet
             << "\n Folder = " << folderPath
             << "\n File = " << filePath
             << "\n ----- ----- ----- ----- ----- ----- \n" ;
}

void ArchiveTestWgt::on_testBtn_2_clicked()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString folderPath = appPath + QDir::separator() + QString("LogUnZip") + QDir::separator();
    QString filePath = appPath + QDir::separator() + QString("LogZip.zip");
    bool bRet = decompressZipToFolder(filePath, folderPath);

    qDebug() << "----- ----- ----- ----- ----- ----- \n"
             << __FUNCTION__
             << "\n Ret = " << bRet
             << "\n Folder = " << folderPath
             << "\n File = " << filePath
             << "\n ----- ----- ----- ----- ----- ----- \n" ;
}

void ArchiveTestWgt::on_testBtn_3_clicked()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString folderPath = appPath + QDir::separator() + QString("Log") + QDir::separator();
    QString filePath = appPath + QDir::separator() + QString("LogTar.tar");
    bool bRet = compressFolderToTar(folderPath, filePath);

    qDebug() << "----- ----- ----- ----- ----- ----- \n"
             << __FUNCTION__
             << "\n Ret = " << bRet
             << "\n Folder = " << folderPath
             << "\n File = " << filePath
             << "\n ----- ----- ----- ----- ----- ----- \n" ;
}

void ArchiveTestWgt::on_testBtn_4_clicked()
{
    QString appPath = QCoreApplication::applicationDirPath();
    QString folderPath = appPath + QDir::separator() + QString("LogUnTar") + QDir::separator();
    QString filePath = appPath + QDir::separator() + QString("LogTar.tar");
    bool bRet = decompressTarToFolder(filePath, folderPath);

    qDebug() << "----- ----- ----- ----- ----- ----- \n"
             << __FUNCTION__
             << "\n Ret = " << bRet
             << "\n Folder = " << folderPath
             << "\n File = " << filePath
             << "\n ----- ----- ----- ----- ----- ----- \n" ;
}




