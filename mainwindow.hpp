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

	void updatePlot(double accelX, double accelY, double gyroX, double gyroY);

private:
	Ui::MainWindow *ui;
	QTcpSocket* m_tcpSocket;

	QVector<QPointF> m_accelXPoints, m_accelYPoints, m_gyroXPoints, m_gyroYPoints;
	QwtPointSeriesData* m_accelXData, *m_accelYData, *m_gyroXData, *m_gyroYData;
	QwtPlotCurve* m_accelXCurve, *m_accelYCurve, *m_gyroXCurve, *m_gyroYCurve;
	int m_counter;
};

#endif // MAINWINDOW_HPP
