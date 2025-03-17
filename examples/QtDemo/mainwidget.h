#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private slots:
    void on_testBtn_1_clicked();
    void on_testBtn_2_clicked();
    void on_testBtn_3_clicked();
    void on_testBtn_4_clicked();

private:
    Ui::MainWidget *ui;
};
#endif // MAINWIDGET_H
