#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_tcpSocket = new QTcpSocket(this);
	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onTcpRead()));

	connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectToServer()));
	connect(ui->disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectToServer()));
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
