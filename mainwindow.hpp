#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTcpSocket>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>


namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void onTcpRead();
	void connectToServer();
	void disconnectToServer();

	void updatePlot();

private:
	Ui::MainWindow *ui;
	QTcpSocket* m_tcpSocket;

	QVector<QPointF> m_points;
	QwtPointSeriesData* m_plotData;
	int m_counter;
};

#endif // MAINWINDOW_HPP
