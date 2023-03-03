#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	QSurfaceFormat fmt;
	fmt.setDepthBufferSize(24);
	//smoother display
	fmt.setSamples(16);
	fmt.setVersion(3, 2);
	fmt.setProfile(QSurfaceFormat::CoreProfile);
	fmt.setSwapInterval(0);
	QSurfaceFormat::setDefaultFormat(fmt);

	MainWindow w;
	w.show();
	return a.exec();
}
