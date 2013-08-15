#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QTime>


void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	Q_UNUSED(context);

	QString time = QTime::currentTime().toString("hh:mm:ss.zzz");

	QString txt;
	switch (type)
	{
	case QtDebugMsg:
		txt = QString("%1 Debug: %2").arg(time, msg);
		break;

	case QtWarningMsg:
		txt = QString("%1 Warning: %2").arg(time, msg);
		break;

	case QtCriticalMsg:
		txt = QString("%1 Critical: %2").arg(time, msg);
		break;

	case QtFatalMsg:
		txt = QString("%1 Fatal: %2").arg(time, msg);
		abort();
	}

	QFile outFile("Studio.log");
	outFile.open(QIODevice::WriteOnly | QIODevice::Append);
	QTextStream ts(&outFile);
	ts << txt << endl;
}

int main(int argc, char *argv[])
{
	// TODO install message handler only on certain command line param
	qInstallMessageHandler(myMessageHandler);
	QApplication a(argc, argv);

	MainWindow w;
	w.show();
	
	return a.exec();
}
