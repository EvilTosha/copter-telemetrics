#include "mainwindow.hpp"
#include <QApplication>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>

int main(int argc, char *argv[])
{
	/* disclamer:
	*  this whole application is just for testing purposes,
	*  so it has a lot of lines of hardcode and very bad code.
	*  Don't take it seriously.
	*/ 
	QApplication a(argc, argv);
	MainWindow mw;
	mw.show();
	return a.exec();
}
