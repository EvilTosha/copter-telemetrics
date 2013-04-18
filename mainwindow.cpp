#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QTimer>

static const int s_plot_length = 100;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_accelXPoints(100, QPointF(0, 0)), m_accelYPoints(100, QPointF(0, 0)),
	m_gyroXPoints(100, QPointF(0, 0)), m_gyroYPoints(100, QPointF(0, 0)),
	m_counter(0)
{
	ui->setupUi(this);

	m_tcpSocket = new QTcpSocket(this);
	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onTcpRead()));

	connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectToServer()));
	connect(ui->disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectToServer()));

	// plotting setup
	m_accelXData = new QwtPointSeriesData();
	m_accelYData = new QwtPointSeriesData();
	m_accelXCurve = new QwtPlotCurve("Accel X");
	m_accelYCurve = new QwtPlotCurve("Accel Y");
	m_accelXCurve->setPen( QPen( Qt::black ) );
	m_accelYCurve->setPen( QPen( Qt::red));
	m_accelXCurve->attach( ui->plot );
	m_accelYCurve->attach(ui->plot);

	m_gyroXData = new QwtPointSeriesData();
	m_gyroYData = new QwtPointSeriesData();
	m_gyroXCurve = new QwtPlotCurve("Gyro X");
	m_gyroYCurve = new QwtPlotCurve("Gyro Y");
	m_gyroXCurve->setPen( QPen( Qt::green ) );
	m_gyroYCurve->setPen( QPen( Qt::blue));
	m_gyroXCurve->attach( ui->plot );
	m_gyroYCurve->attach(ui->plot);

	ui->plot->setCanvasBackground(Qt::white);
	// Axis
	ui->plot->setAxisTitle( QwtPlot::xBottom, "Seconds" );
	ui->plot->setAxisTitle( QwtPlot::yLeft, "Values" );
	ui->plot->setAxisScale( QwtPlot::yLeft, -256, 256 );

	updatePlot(0, 0, 0, 0);

//	QTimer* t = new QTimer(this);
//	t->setInterval(10);
//	connect(t, SIGNAL(timeout()), this, SLOT(updatePlot()));
//	t->start();
}

void MainWindow::updatePlot(double accelX, double accelY, double gyroX, double gyroY)
{
	for (int i = 0; i < s_plot_length - 1; ++i) {
		m_accelXPoints[i] = m_accelXPoints[i + 1];
		m_accelYPoints[i] = m_accelYPoints[i + 1];
		m_gyroXPoints[i] = m_gyroXPoints[i + 1];
		m_gyroYPoints[i] = m_gyroYPoints[i + 1];
	}
	m_accelXPoints[s_plot_length - 1] = QPointF(m_counter, accelX);
	m_accelYPoints[s_plot_length - 1] = QPointF(m_counter, accelY);
	m_gyroXPoints[s_plot_length - 1] = QPointF(m_counter, gyroX);
	m_gyroYPoints[s_plot_length - 1] = QPointF(m_counter, gyroY);

	++m_counter;
	m_accelXData->setSamples(m_accelXPoints);
	m_accelYData->setSamples(m_accelYPoints);
	m_accelXCurve->setData( m_accelXData );
	m_accelYCurve->setData(m_accelYData);

	m_gyroXData->setSamples(m_gyroXPoints);
	m_gyroYData->setSamples(m_gyroYPoints);
	m_gyroXCurve->setData( m_gyroXData );
	m_gyroYCurve->setData(m_gyroYData);

	ui->plot->setAxisScale( QwtPlot::xBottom, m_counter - 100, m_counter );

	ui->plot->replot();
}

MainWindow::~MainWindow()
{
	delete ui;
	m_tcpSocket->close();
}

void MainWindow::onTcpRead()
{
	if (!m_tcpSocket->isReadable())
		return;
	while (m_tcpSocket->bytesAvailable() > 0) {
		char data[1024];
		m_tcpSocket->readLine(data, 1024);
		QString line(data);
		if (!line.isEmpty()) {
			ui->logTextBrowser->setText(line);
			QStringList values = line.split(",");
			if (values.length() < 14) {
				qDebug() << "Wrong format of debug input";
			}
			else {
				ui->powerAllLcd->display(values.at(9).toInt());
				ui->powerX1Lcd->display(values.at(10).toInt());
				ui->powerX2Lcd->display(values.at(11).toInt());
				ui->powerY1Lcd->display(values.at(12).toInt());
				ui->powerY2Lcd->display(values.at(13).toInt());
				double ax = values.at(0).toDouble();
				double ay = values.at(1).toDouble();
				double gx = values.at(3).toDouble();
				double gy = values.at(4).toDouble();
				updatePlot(ax, ay, gx, gy);
				ui->xSlider->setValue(floor(ax));
				ui->ySlider->setValue(floor(ay));
			}
		}
		ui->logTextBrowser->update();
	}
}

void MainWindow::connectToServer()
{
	QString hostName(ui->hostNameLineEdit->text());
	quint16 port = ui->portLineEdit->text().toUInt();
	m_tcpSocket->connectToHost(hostName, port);
}

void MainWindow::disconnectToServer()
{
	m_tcpSocket->disconnectFromHost();
}


