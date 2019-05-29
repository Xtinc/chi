#include "Chimera.h"
#include "Application.h"
#include <QFile>
#include <QDebug>
#include <QPlainTextEdit>
#include <QDateTime>

QPlainTextEdit *chi=nullptr;

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
	if (chi != nullptr) {
		QString text;
		bool ierror = false;
		switch (type)
		{
		case QtDebugMsg:
			text = "Debug:";
			break;
		case QtWarningMsg:
			text = "Warning:";
			ierror = true;
			break;
		case QtCriticalMsg:
			text = "Critical:";
			ierror = true;
			break;
		case QtFatalMsg:
			text = "Fatal:";
			ierror = true;
			break;
		case QtInfoMsg:
			text = "Info:";
			break;
		default:
			break;
		}
		QString date_time = QDateTime::currentDateTime().toString("[MM-dd hh:mm:ss]");
		if (ierror) {
			chi->appendPlainText(QStringLiteral("%1%2 File:%3 Line: %4 %5").arg(date_time).arg(text).arg(context.file).arg(context.line).arg(msg));
		}
		else
		{
			chi->appendPlainText(QStringLiteral("%1%2 %3").arg(date_time).arg(text).arg(msg));
		}
	}
};

static void initStyleSheet(QApplication& a)
{
	//Q_INIT_RESOURCE(ads); // If static linked.
	QFile f(":/ads/Resources/default-windows.css");
	if (f.open(QFile::ReadOnly))
	{
		const QByteArray ba = f.readAll();
		f.close();
		a.setStyleSheet(QString(ba));
	}
	else
	{
		qWarning()<<"stylesheet is not loaded.";
	}
	for (auto i : { "consola","consolab","consolai","consolaz" }) {
		int k=QFontDatabase::addApplicationFont(":/font/Resources/" + QString(i) + ".ttf");
		if (k == -1) {
			qWarning() << "load "<<i<<" failed.";
		}
	}
	QFont font;
	font.setFamily("consolas");
	a.setFont(font);
}

int main(int argc, char *argv[])
{
	Application a(argc, argv);
	a.setQuitOnLastWindowClosed(true);
	Chimera w;
	w.setWindowIcon(QIcon(":/Chimera/Resources/bitbug_favicon.ico"));
	chi = w.console();
	qInstallMessageHandler(outputMessage);
	initStyleSheet(a);
	w.show();
	return a.exec();
}
