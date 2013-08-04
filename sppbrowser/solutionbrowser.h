#ifndef SOLUTIONBROWSER_H
#define SOLUTIONBROWSER_H

#include "sppbrowser_global.h"
#include <QWidget>
#include <QTreeWidget>
#include <QThread>
#include "data.h"
#include "solutionscanner.h"

namespace Ui {
class SolutionBrowser;
}

/*!
 * \brief The SolutionBrowser widget class
 *
 * This widget displays the solution structure of a Visual Studio
 * solution. It will include the projects and file structure and is
 * able to emit signals when a solution item is double clicked.
 */
class SPPBROWSERSHARED_EXPORT SolutionBrowser : public QWidget
{
	Q_OBJECT
	QThread workerThread;

public:
	explicit SolutionBrowser(QWidget *parent = 0);
	~SolutionBrowser();
	
private:
	Ui::SolutionBrowser* ui;
	QList<ProjectPtr>* _results;

	void updateTree(QList<ProjectPtr> *results);

private slots:
	void handleProgress(int step, int progress);
	void handleResults(QList<ProjectPtr>* results);
	void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_openButton_clicked();

signals:
	void operate(const QString & solutionFile);
	void itemDoubleClicked(const QString& file);
};

#endif // SOLUTIONBROWSER_H
