#ifndef ARCHIVETESTWGT_H
#define ARCHIVETESTWGT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ArchiveTestWgt; }
QT_END_NAMESPACE

class ArchiveTestWgt : public QWidget
{
    Q_OBJECT

public:
    ArchiveTestWgt(QWidget *parent = nullptr);
    ~ArchiveTestWgt();

private:
    bool compressFolderToZip(const QString &folderPath, const QString &archivePath);
    bool decompressZipToFolder(const QString &archivePath, const QString &outputFolderPath);
    bool compressFolderToTar(const QString &folderPath, const QString &tarFilePath);
    bool decompressTarToFolder(const QString &tarFilePath, const QString &outputFolderPath);

    bool compressFolderToZipSys(QString folderPath, QString archivePath);
    bool decompressZipToFolderSys(QString archivePath, QString folderPath);

private slots:
    void on_testBtn_1_clicked();
    void on_testBtn_2_clicked();
    void on_testBtn_3_clicked();
    void on_testBtn_4_clicked();

private:
    Ui::ArchiveTestWgt *ui;
};
#endif // ARCHIVETESTWGT_H
