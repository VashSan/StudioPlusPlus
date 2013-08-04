#include "solutionscanner.h"

#include <QtXml>
#include <QFile>
#include <stdexcept>
#include "data.h"

void SolutionScanner::doWork(const QString &solutionFile)
{
	// make sure our members are safe
	QMutexLocker lock(&_mutex);

	QFileInfo fileInfo(solutionFile);
	_solutionPath = fileInfo.canonicalPath();
	_solutionFile = fileInfo.fileName();
	_projectList.clear();

	scanSolution(solutionFile);

	const int projectCount = _projectList.length();
	for (int i = 0; i < projectCount; i++)
	{
		ProjectPtr project = _projectList.at(i);
		readProject(project);

		const int relativeProgress = (int)((i+1) /  projectCount);
		emit progress(2, relativeProgress * 100);
	}

	emit resultReady(&_projectList);
}

void SolutionScanner::scanSolution(const QString& solutionFile)
{
	QFile file(solutionFile);
	if( !file.open(QFile::ReadOnly | QFile::Text))
	{
		// TODO emit error signal instead
		throw std::runtime_error("Solution file could not be opened");
	}

	const qint64 fileSize = file.size(); // estimate scan progress

	while(!file.atEnd())
	{
		QByteArray lineBytes = file.readLine();
		QString line(lineBytes);

		if (line.startsWith("Project"))
		{
			ProjectPtr p = parseProjectDeclaration(line);
			_projectList.append(p);
		}

		const qint64 relativePosition = (qint64)(file.pos() / fileSize);
		emit progress(1, relativePosition * 100);
	}
}

ProjectPtr SolutionScanner::parseProjectDeclaration(const QString& line)
{
	QString uuidRegEx("{[0-9A-F]{8}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{4}-[0-9A-F]{12}}");
	QString projectRegEx = QString("Project\\(\"(%1)\"\\)\\s*=\\s*\"(.*?)\"\\s*,\\s*\"(.*?)\"\\s*,\\s*\"(%2)\"").arg(uuidRegEx, uuidRegEx);

	ProjectPtr project(new ProjectDeclaration());
	QRegularExpression regEx(projectRegEx);
	QRegularExpressionMatch match = regEx.match(line);
	if (match.hasMatch())
	{
		QUuid type(match.captured(1));
		project->type = type;

		project->name = match.captured(2);
		project->file = match.captured(3);

		QUuid id(match.captured(4));
		project->id = id;
	}

	return project;
}

void SolutionScanner::readProject(ProjectPtr project)
{
	QDomDocument dom;

	QDir dir(_solutionPath);
	QFileInfo fileInfo(dir, project->file);
	QString projectPath = fileInfo.canonicalPath();
	QString projectFilePath = fileInfo.canonicalFilePath();
	QFile file(projectFilePath);
	if (!file.open(QFile::ReadOnly))
	{
		// TODO emit error signal instead
		throw std::runtime_error("Project file could not be opened");
	}

	dom.setContent(&file);

	QDomNodeList itemGroupList = dom.elementsByTagName("ItemGroup");
	for (int i = 0; i < itemGroupList.length(); i++)
	{
		QDomNode itemGroup = itemGroupList.at(i);
		QDomNodeList itemList = itemGroup.childNodes();
		for(int j = 0; j < itemList.length(); j++)
		{
			QDomNode item = itemList.at(j);
			QDomNode fileNameNode = item.attributes().namedItem("Include");
			if( ! fileNameNode.isNull() )
			{
				QDomAttr fileNameAttribute = fileNameNode.toAttr();

				SourceFile s;
				s.file = fileNameAttribute.value();

				QDir projectDir(projectPath);
				QFileInfo sourceFileInfo(projectDir, s.file);
				if (sourceFileInfo.isFile())
				{
					s.filePath = sourceFileInfo.canonicalFilePath();
				}

				QString itemName = item.nodeName();
				if (itemName == "None")
				{
					s.type = SourceFile::Unspecified;
				}
				else if (itemName == "ClInclude")
				{
					s.type = SourceFile::CppHeader;
				}
				else if (itemName == "ClCompile")
				{
					s.type = SourceFile::CppSource;
				}
				else
				{
					s.type = SourceFile::Unknown;
				}

				project->sourceList.append(s);
			}
		}
	}
}
