#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QTimer>

static const int s_plot_length = 100;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_points(100, QPointF(0, 0)), m_altPoints(100, QPointF(0, 0)),
	m_counter(0)
{
	ui->setupUi(this);

	m_tcpSocket = new QTcpSocket(this);
	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onTcpRead()));

	connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectToServer()));
	connect(ui->disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectToServer()));

	// plotting setup
	m_plotData = new QwtPointSeriesData();
	m_altPlotData = new QwtPointSeriesData();
	m_plotCurve = new QwtPlotCurve( "Data Moving Right" );
	m_altPlotCurve = new QwtPlotCurve();
	m_plotCurve->setPen( QPen( Qt::black ) );
	m_altPlotCurve->setPen( QPen( Qt::red));
	m_plotCurve->attach( ui->plot );
	m_altPlotCurve->attach(ui->plot);

	ui->plot->setCanvasBackground(Qt::white);
	// Axis
	ui->plot->setAxisTitle( QwtPlot::xBottom, "Seconds" );
	ui->plot->setAxisTitle( QwtPlot::yLeft, "Values" );
	ui->plot->setAxisScale( QwtPlot::yLeft, -100, 100 );

	updatePlot(0, 0);

//	QTimer* t = new QTimer(this);
//	t->setInterval(10);
//	connect(t, SIGNAL(timeout()), this, SLOT(updatePlot()));
//	t->start();
}

void MainWindow::updatePlot(double val, double altVal)
{
	for (int i = 0; i < s_plot_length - 1; ++i) {
		m_points[i] = m_points[i + 1];
		m_altPoints[i] = m_altPoints[i + 1];
	}
	m_points[s_plot_length - 1] = QPointF(m_counter, val);
	m_altPoints[s_plot_length - 1] = QPointF(m_counter, altVal);

	++m_counter;
	m_plotData->setSamples(m_points);
	m_altPlotData->setSamples(m_altPoints);
	m_plotCurve->setData( m_plotData );
	m_altPlotCurve->setData(m_altPlotData);
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
			if (values.length() < 11) {
				qDebug() << "Wrong format of debug input";
			}
			else {
				ui->powerAllLcd->display(values.at(6).toInt());
				ui->powerX1Lcd->display(values.at(7).toInt());
				ui->powerX2Lcd->display(values.at(8).toInt());
				ui->powerY1Lcd->display(values.at(9).toInt());
				ui->powerY2Lcd->display(values.at(10).toInt());
				updatePlot(values.at(0).toDouble(), values.at(1).toDouble());
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


