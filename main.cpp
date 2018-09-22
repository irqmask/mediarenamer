#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QCoreApplication::setOrganizationName("CrisSoft");
    QCoreApplication::setOrganizationDomain("crissoft.de");
    QCoreApplication::setApplicationName("MediaRenamer");

    w.show();

    return a.exec();
}
