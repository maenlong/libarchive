#include "archivetestwgt.h"
#include "ui_archivetestwgt.h"

#include <archive.h>
#include <archive_entry.h>

#include <QDir>
#include <QFileInfoList>
#include <QString>
#include <QDebug>
#include <QProcess>

bool ArchiveTestWgt::compressFolderToZip(const QString &folderPath, const QString &archivePath)
{
#ifdef HAVE_LIB_ARCHIVE
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
#else
    bool bRet = compressFolderToZipSys(folderPath, archivePath);
    return bRet;
#endif
}

bool ArchiveTestWgt::decompressZipToFolder(const QString &archivePath, const QString &outputFolderPath)
{
#ifdef HAVE_LIB_ARCHIVE
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
#else
    bool bRet = decompressZipToFolderSys(archivePath, folderPath);
    return bRet;
#endif
}



bool ArchiveTestWgt::compressFolderToTar(const QString &folderPath, const QString &tarFilePath)
{
#ifdef HAVE_LIB_ARCHIVE
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
#else
    return false;
#endif
}

bool ArchiveTestWgt::decompressTarToFolder(const QString &tarFilePath, const QString &outputFolderPath)
{
#ifdef HAVE_LIB_ARCHIVE
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
#else
    return false;
#endif
}



bool ArchiveTestWgt::compressFolderToZipSys(QString folderPath, QString archivePath)
{
    folderPath = QDir::cleanPath(folderPath);
    archivePath = QDir::cleanPath(archivePath);

    // 检查输入路径是否存在
    QDir dir(folderPath);
    if (!dir.exists())
    {
        qDebug() << "Folder does not exist:" << folderPath;
        return false;
    }

    // 如果压缩文件已经存在，先删除它
    QFile archiveFile(archivePath);
    if (archiveFile.exists())
    {
        if (!archiveFile.remove())
        {
            qDebug() << "Failed to remove existing archive file:" << archivePath;
            return false;
        }
    }

    QProcess process;
    QString command;

#ifdef Q_OS_WIN
    // Windows 平台，使用 PowerShell 的 Compress-Archive
    command = QString("powershell -Command Compress-Archive -Path \"%1\" -DestinationPath \"%2\"")
                  .arg(folderPath.replace("/", "\\"))  // PowerShell 使用 Windows 风格路径
                  .arg(archivePath.replace("/", "\\"));
#elif defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    // macOS 和 Linux 平台，调整 zip 命令的工作路径
    QDir parentDir = dir.absolutePath() + "/..";  // 获取上级目录
    QString folderName = dir.dirName();          // 获取当前文件夹名
    process.setWorkingDirectory(parentDir.absolutePath()); // 显式设置工作目录

    command = QString("zip -r \"%1\" \"%2\"")
                  .arg(archivePath)  // 压缩文件路径
                  .arg(folderName);  // 当前文件夹名
#else
    qDebug() << "Unsupported platform!";
    return false;
#endif

    qDebug() << "Executing command:" << command;

    // 执行命令
    process.start(command);
    if (!process.waitForFinished())
    {
        qDebug() << "Compression failed:" << process.errorString();
        qDebug() << "Standard Output:" << process.readAllStandardOutput();
        qDebug() << "Standard Error:" << process.readAllStandardError();
        return false;
    }

    // 检查返回状态
    if (process.exitCode() != 0)
    {
        qDebug() << "Compression command failed with exit code:" << process.exitCode();
        qDebug() << "Standard Output:" << process.readAllStandardOutput();
        qDebug() << "Standard Error:" << process.readAllStandardError();
        return false;
    }

    // 检查压缩文件是否生成
    if (!QFile::exists(archivePath))
    {
        qDebug() << "Zip file not found at expected location:" << archivePath;
        return false;
    }

    qDebug() << "Compression successful!";
    return true;
}



bool ArchiveTestWgt::decompressZipToFolderSys(QString archivePath, QString folderPath)
{
    // 清理路径
    archivePath = QDir::cleanPath(archivePath);
    folderPath = QDir::cleanPath(folderPath);

    // 确保 ZIP 文件存在
    QFile archiveFile(archivePath);
    if (!archiveFile.exists())
    {
        qDebug() << "Archive file does not exist:" << archivePath;
        return false;
    }

    QDir outputDir(folderPath);

    /*
    // 如果目标文件夹存在，删除整个文件夹
    if (outputDir.exists())
    {
        if (!outputDir.removeRecursively())
        {
            qDebug() << "Failed to remove existing folder:" << folderPath;
            return false;
        }
    }

    // 创建目标文件夹
    if (!outputDir.mkpath("."))
    {
        qDebug() << "Failed to create output folder:" << folderPath;
        return false;
    }
    */

    QProcess process;
    QString command;

#ifdef Q_OS_WIN
    // Windows 平台，使用 PowerShell 解压
    command = QString("powershell -Command Expand-Archive -Path \"%1\" -DestinationPath \"%2\"")
                  .arg(archivePath.replace("/", "\\"))  // Windows 使用反斜杠路径
                  .arg(folderPath.replace("/", "\\"));
#elif defined(Q_OS_MAC) || defined(Q_OS_LINUX)
    // macOS 和 Linux 平台，使用 unzip 命令并移除路径
    command = QString("unzip -o \"%1\" -d \"%2\"")
                  .arg(archivePath)         // 压缩文件路径
                  .arg(folderPath);         // 输出文件夹路径
#else
    qDebug() << "Unsupported platform!";
    return false;
#endif

    qDebug() << "Executing command:" << command;

    // 执行命令
    process.start(command);
    if (!process.waitForFinished())
    {
        qDebug() << "Decompression failed:" << process.errorString();
        qDebug() << "Standard Output:" << process.readAllStandardOutput();
        qDebug() << "Standard Error:" << process.readAllStandardError();
        return false;
    }

    // 检查返回状态
    if (process.exitCode() != 0)
    {
        qDebug() << "Decompression command failed with exit code:" << process.exitCode();
        qDebug() << "Standard Output:" << process.readAllStandardOutput();
        qDebug() << "Error output:" << process.readAllStandardError();
        return false;
    }

    // 检查输出文件夹是否创建成功
    if (!outputDir.exists())
    {
        qDebug() << "Output folder not found after decompression:" << folderPath;
        return false;
    }

    qDebug() << "Decompression successful!";
    return true;
}





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


