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

	void updatePlot(double val, double altVal);

private:
	Ui::MainWindow *ui;
	QTcpSocket* m_tcpSocket;

	QVector<QPointF> m_points, m_altPoints;
	QwtPointSeriesData* m_plotData, *m_altPlotData;
	QwtPlotCurve* m_plotCurve, *m_altPlotCurve;
	int m_counter;
};

#endif // MAINWINDOW_HPP
