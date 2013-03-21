#include "mainwindow.hpp"
#include <QApplication>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow mw;
	mw.show();
	return a.exec();
}
