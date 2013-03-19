#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTcpSocket>

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

private:
	Ui::MainWindow *ui;
	QTcpSocket* m_tcpSocket;
};

#endif // MAINWINDOW_HPP
