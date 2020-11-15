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
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow w;
    w.show();
    return a.exec();
}
