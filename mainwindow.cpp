#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_tcpSocket = new QTcpSocket(this);
	connect(m_tcpSocket, SIGNAL(readyRead()), this, SLOT(onTcpRead()));

	ui->logTextBrowser->setText("ololo");
	ui->logTextBrowser->update();
	connect(ui->connectButton, SIGNAL(clicked()), this, SLOT(connectToServer()));
	connect(ui->disconnectButton, SIGNAL(clicked()), this, SLOT(disconnectToServer()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::onTcpRead()
{
	if (!m_tcpSocket->isReadable())
		return;
	while (m_tcpSocket->bytesAvailable() > 0) {
		char data[1024];
		m_tcpSocket->readLine(data, 1024);
		QString line(data);
		if (!line.isEmpty())
			ui->logTextBrowser->setText(line);
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
