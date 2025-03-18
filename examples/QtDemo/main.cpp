#include <QApplication>
#include "archivetestwgt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ArchiveTestWgt w;
    w.show();

    return a.exec();
}
