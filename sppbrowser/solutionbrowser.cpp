#include "solutionbrowser.h"
#include "ui_solutionbrowser.h"
#include <QFileDialog>
#include <QWidget>
#include <QThread>
#include "solutionscanner.h"


SolutionBrowser::SolutionBrowser(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SolutionBrowser)
{
	ui->setupUi(this);

	SolutionScanner* worker = new SolutionScanner();
	worker->moveToThread(&workerThread);

	connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
	connect(this, &SolutionBrowser::operate, worker, &SolutionScanner::doWork);
	connect(worker, &SolutionScanner::resultReady, this, &SolutionBrowser::handleResults);
	connect(worker, &SolutionScanner::progress, this, &SolutionBrowser::handleProgress);

	workerThread.start();
}

SolutionBrowser::~SolutionBrowser()
{
	workerThread.quit();
	workerThread.wait();
	delete ui;
}

void SolutionBrowser::handleResults(QList<ProjectPtr> *results)
{
	_results = results;
	updateTree(results);
}

void SolutionBrowser::handleProgress(int step, int progress)
{
	// assuming two steps
	progress /= 2;

	if (step == 2)
	{
		progress += 50;
	}

	ui->progressBar->setValue(progress);
}

void SolutionBrowser::updateTree(QList<ProjectPtr> *results)
{
	ui->treeWidget->clear();
	ui->treeWidget->setColumnCount(2);
	QList<QTreeWidgetItem *> items;
	for (int i = 0; i < results->count(); ++i)
	{
		const ProjectPtr p = results->at(i);

		QStringList list;
		list.append(p->name);
		list.append(p->file);

		auto item = new QTreeWidgetItem((QTreeWidget*)0, list);
		for (int k = 0; k < p->sourceList.length(); k++)
		{
			SourceFile f = p->sourceList.at(k);

			QStringList childList;
			childList.append(f.file);

			auto sourceItem = new QTreeWidgetItem((QTreeWidget*)0, childList);

			QVariant toolTipData(f.filePath);
			sourceItem->setData(0, Qt::ToolTipRole, toolTipData);

			item->addChild(sourceItem);
		}

		items.append(item);
	}
	ui->treeWidget->insertTopLevelItems(0, items);
}

void SolutionBrowser::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	QVariant data = item->data(0, Qt::ToolTipRole);
	QString file = data.toString();
	itemDoubleClicked(file);
}

void SolutionBrowser::on_openButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		"Open Solution", "c:/", "Visual Studio Solution (*.sln)");
	if (! (fileName.isNull() || fileName.isEmpty()) )
	{
		operate(fileName);
	}
}
