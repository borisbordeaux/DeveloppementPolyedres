#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //smoother display
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setSamples(16);
    fmt.setVersion(3,2);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    int *in = nullptr;
    delete in;

    MainWindow w;
    w.show();
    return a.exec();
}
