#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "solutionbrowser.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	//QString fileName = "C:\\Users\\Alex\\Desktop\\plugindemo\\NppPluginTemplate.sln";

	browser = new SolutionBrowser();
	connect(browser, &SolutionBrowser::itemDoubleClicked, this, &MainWindow::on_itemDoubleClicked);

	ui->gridLayout->addWidget(browser, 0, 0);
}

MainWindow::~MainWindow()
{
	delete browser;
	delete ui;
}

void MainWindow::on_itemDoubleClicked(const QString& filePath)
{
	ui->filePath->setText(filePath);

	QFile file(filePath);
	if (file.open(QFile::ReadOnly | QFile::Text))
	{
		QByteArray bytes = file.readAll();
		QString content(bytes);
		ui->fileContent->setText(content);
	}
}
