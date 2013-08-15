#ifndef SOLUTIONSCANNER_H
#define SOLUTIONSCANNER_H

#include <QObject>
#include "data.h"

class SolutionScanner : public QObject
{
	Q_OBJECT

public slots:
	void doWork(const QString &solutionFile);

signals:
	void resultReady(QList<ProjectPtr>* result);
	void progress(int step, int progress);

private:
	QString _solutionFile;
	QString _solutionPath;
	QList<ProjectPtr> _projectList;
	mutable QMutex _mutex;

	/// Returns true if the solution file was scanned successfully.
	bool scanSolution(const QString& parameter);

	ProjectPtr parseProjectDeclaration(const QString& line);

	void readProject(ProjectPtr project);
};

#endif // SOLUTIONSCANNER_H
