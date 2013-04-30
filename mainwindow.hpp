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
	
	enum ControlCommand {
		PowerUp1 = 0,
		PowerUp2,
		PowerMax,
		PowerDown1,
		PowerDown2,
		PowerMin,
		EmergencyStop,
		StartupAdjust,
		NUM_COMMANDS
	};
	
public slots:
	void onTcpRead();
	void onControlRead();
	void connectToServer();
	void disconnectFromServer();

	void updatePlot(double accelX, double accelY, double gyroX, double gyroY);
	
	void onPowerChange(int val);
	void handleControlButton();
	void controlSend(MainWindow::ControlCommand command);
	
private:
	Ui::MainWindow *ui;
	QTcpSocket* m_tcpSocket;
	QTcpSocket* m_controlSocket;

	QVector<QPointF> m_accelXPoints, m_accelYPoints, m_gyroXPoints, m_gyroYPoints;
	QwtPointSeriesData* m_accelXData, *m_accelYData, *m_gyroXData, *m_gyroYData;
	QwtPlotCurve* m_accelXCurve, *m_accelYCurve, *m_gyroXCurve, *m_gyroYCurve;
	int m_counter;
	int m_power;
};

#endif // MAINWINDOW_HPP
