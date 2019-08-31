#include "InputDeck.h"
#include "Editor.h"
#include "Chart.h"
#include "clib.h"
#include "Application.h"
#include "simplecpp.h"
#include <fstream>
#include <cstring>
#include <QTextCodec>
#include <QApplication>
#include <QTextStream>
#include <QProcess>
#include <QStringList>
#include <QDir>
#include <QRegularExpression>
#include <QFileInfo>
#include <QByteArray>
#include <QDebug>
#include <QDialog>
#include <QVBoxLayout>
#include <QDesktopWidget>

static int count = 0;

QString trim(const QString &str)
{
	qInfo() << "linking...";
	QByteArray ba = str.toLocal8Bit();
	char *prefile = ba.data();
	simplecpp::DUI dui;
	simplecpp::OutputList outputList;
	std::vector<std::string> files;
	std::ifstream f(prefile);
	simplecpp::TokenList rawtokens(f, files, prefile, &outputList);
	rawtokens.removeComments();
	std::map<std::string, simplecpp::TokenList*> included = simplecpp::load(rawtokens, files, dui, &outputList);
	for (std::pair<std::string, simplecpp::TokenList *> i : included)
		i.second->removeComments();
	simplecpp::TokenList outputTokens(files);
	simplecpp::preprocess(outputTokens, rawtokens, files, included, dui, &outputList);

	QString Kstr = QString::fromLocal8Bit(outputTokens.stringify().c_str());

	/*QString encoding = detectEncode(filename);

	file.open(QFile::ReadOnly);
	QTextStream in(&file);
	QTextCodec *codec_for_encoding = QTextCodec::codecForName(encoding.toUtf8());
	in.setCodec(codec_for_encoding);

	QString str = in.readAll();
	file.close();
	trim(str);
	*/
	QString preout;
	for (const simplecpp::Output &output : outputList) {
		preout = "";
		preout = preout + QString::fromStdString(output.location.file()) + ':' + QString::number(output.location.line) + ": ";
		switch (output.type) {
		case simplecpp::Output::CERROR:
			preout = preout + "#error: ";
			break;
		case simplecpp::Output::WARNING:
			preout = preout + "#warning: ";
			break;
		case simplecpp::Output::MISSING_HEADER:
			preout = preout + "missing header: ";
			break;
		case simplecpp::Output::INCLUDE_NESTED_TOO_DEEPLY:
			preout = preout + "include nested too deeply: ";
			break;
		case simplecpp::Output::SYNTAX_ERROR:
			preout = preout + "syntax error: ";
			break;
		case simplecpp::Output::PORTABILITY_BACKSLASH:
			preout = preout + "portability: ";
			break;
		case simplecpp::Output::UNHANDLED_CHAR_ERROR:
			preout = preout + "unhandled char error: ";
			break;
		}
		preout = preout + QString::fromStdString(output.msg);
		qInfo() << preout.toLatin1().data();
	}

	// cleanup included tokenlists
	simplecpp::cleanup(included);
	Kstr.replace(";", "\n");//';' is treated as new line.
	qInfo() << "linked...";
	return Kstr;
	/*if (stack < 0) {
		qWarning()<<"a recursion stack over 20 levels was found.it's usually caused by circular reference.";
		return;
	}
	qInfo()<<"processing marcos...";

	QRegularExpression re1(QStringLiteral("^#define\\s+(\\w+)\\(((\\w+,{0,1})+)\\){([^#]*)}"), QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
	QRegularExpressionMatchIterator h = re1.globalMatch(str);
	while (h.hasNext()) {
		QRegularExpressionMatch match = h.next();
		str.remove(match.captured(0));
		QStringList arglist = match.captured(2).split(",", QString::SkipEmptyParts);
		QString func = match.captured(4);
		int count = 0;
		for (auto j : arglist) {
			func.replace(j, QString("#arg%1").arg(count));
			count = count + 1;
		}
		qInfo()<<"replacing" << match.captured(1).toLocal8Bit().data() << "with\n" << func.toLocal8Bit().data();
		QRegularExpression tmp(match.captured(1) + "\\(((\\S+,{0,1})*)\\)", QRegularExpression::MultilineOption);
		QRegularExpressionMatchIterator tmpi = tmp.globalMatch(str);
		while (tmpi.hasNext()) {
			QString tmpfunc = func;
			QRegularExpressionMatch tmpmatch = tmpi.next();
			qInfo()<<"extanding..."<<tmpmatch.captured(0);
			QStringList arglist = tmpmatch.captured(1).split(",", QString::SkipEmptyParts);
			int dout = 0;
			for (auto k : arglist) {
				tmpfunc.replace(QStringLiteral("#arg%1").arg(dout), k);
				dout = dout + 1;
			}
			str.replace(tmpmatch.captured(0), tmpfunc);
		}
	}
	//todo too complex.
	QRegularExpression re0(QStringLiteral("^#define\\s+(\\w+)\\s+(\\S+)"), QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
	QRegularExpressionMatchIterator i = re0.globalMatch(str);
	while (i.hasNext()) {
		QRegularExpressionMatch match = i.next();
		str.remove(match.captured(0));
		qInfo() << "replacing" << match.captured(1) << "with" << match.captured(2) << "...";
		str.replace(match.captured(1), match.captured(2));
	}

	QRegularExpression re(QStringLiteral("^#include\\s+(\\S+)"), QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
	QRegularExpressionMatchIterator j = re.globalMatch(str);
	while (j.hasNext()) {
		QRegularExpressionMatch match = j.next();
		QFile file(match.captured(1));
		if (!file.open(QFile::ReadOnly)) {
			qWarning() << "can't find file:" << match.captured(1);
		}
		else {
			QFileInfo info(file);
			if (info.size() > 100000) {
				qWarning() << "too large to include:" << match.captured(1);
			}
			else {
				QTextStream in(&file);
				QTextCodec* codec_for_encoding = QTextCodec::codecForName(detectEncode(match.captured(1)).toUtf8());
				in.setCodec(codec_for_encoding);
				QString tmp = in.readAll();
				trim(tmp, stack - 1);
				str.replace(match.captured(0), tmp);
				qInfo() << "link " << match.captured(1);
			}
		}
		file.close();
	}*/
}

QString strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

InputDeck::InputDeck(const QString &uuid, const QString &filename, QObject *parent)
	:QObject(parent), interval_(100), uuid_(uuid), filename_(filename)
{
	workdir_ = filename.split(".").first() + QString::number(count);
	timer_.setInterval(interval_);
	connect(&timer_, &QTimer::timeout, this, &InputDeck::ReadOutput);
	count = count + 1;
}

void InputDeck::check(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QFile::ReadOnly)) {
		qCritical()<<"invalid input: "<< filename;
		return;
	}
	file.close();
	QTemporaryFile tmpfile;
	if (!tmpfile.open()) {
		qCritical() << "can't create temporary file.";
		return;
	}
	QString str = trim(filename);
	QTextStream out(&tmpfile);
	out.setCodec("UTF-8");
	out << str;
	tmpfile.close();
	qInfo() << "compiling...";
	QProcess process;
	connect(&process, &QProcess::readyReadStandardOutput, [&process] {
		qInfo() << process.readAllStandardOutput().data();
	});
	process.start("./bin/CheckInput.exe", QStringList() << tmpfile.fileName() << " " << QDir::currentPath() + "/bin/");
	//TODO:changed with melgen.
	if (!process.waitForFinished()) {
		qCritical() << "errors occured when runnning CheckInput.exe.";
		process.close();
		return;
	};
	if (!process.readAllStandardError().isEmpty()) {
		qCritical() << "error";
		return;
	}
	process.close();
	qInfo() << "compiled.";
}

InputDeck::~InputDeck()
{
	
}

int InputDeck::Interval() const
{
	return interval_;
}

void InputDeck::SetInterval(int k)
{
	interval_ = (k > 10) ? k : 100;
}

QString InputDeck::WorkDir() const
{
	return workdir_;
}

void InputDeck::SetWorkDir(const QString &filepath)
{
	if (!filepath.isEmpty()) {
		workdir_ = filepath + QDir::separator() + strippedName(filename_).split(".").first() + QString::number(count);
	}
}

QString InputDeck::uuid() const
{
	return uuid_;
}

bool InputDeck::isRunning()
{
	return process_.state() == QProcess::Running;
}

void InputDeck::Run()
{
	if (init()) {
		if (runMelgen()) {
			if (runMelcor()) {
				timer_.start();
				emit start(uuid_);
			}
		}
	}
}

void InputDeck::destroy()
{
	if (process_.state() == QProcess::Running) {
		timer_.stop();
		disconnect(&process_, nullptr, nullptr, nullptr);
		process_.kill();
		emit aborted(uuid_);
	}
}

void InputDeck::showChart()
{
	QDialog *dig = new QDialog(nullptr, Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	dig->setAttribute(Qt::WA_DeleteOnClose);
	QWChartView *view = new QWChartView(dig);
	Chart *chart = new Chart();
	connect(this, &InputDeck::chartmessage, chart, &Chart::handleTimeout);
	view->setChart(chart);
	view->setMinimumSize(myApp()->desktop()->width()/4,myApp()->desktop()->height()/4);
	QVBoxLayout *layout = new QVBoxLayout(dig);
	dig->setWindowTitle(uuid_);
	layout->addWidget(view);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSizeConstraint(QLayout::SetFixedSize);
	dig->setLayout(layout);
	dig->show();
}

void InputDeck::ReadOutput()
{
	if (thandle > 0)
	{
		QString str = ReadSharedMemory(thandle);
		QStringList list= str.split("#", QString::SkipEmptyParts);
		if (!list.isEmpty()) {
			if (list.first().contains("progress")) {
				emit curprogress(uuid_, list.first().split("$").last());
				list.removeFirst();
			};
		}
		emit chartmessage(list.join("#"));
	}
}

bool InputDeck::init()
{
	QFile file(filename_);
	if (!file.open(QFile::ReadOnly)) {
		qWarning() << uuid_ << "invalid Inputcard." << filename_;
		return false;
	}
	file.close();
	file_ = new QTemporaryFile(this);
	if (!file_->open()) {
		qWarning() << uuid_ << "can't create temporary file.";
		file.close();
		return false;
	}
	file.close();
	//todo detect encoding could spend less.
	//I/O opt

	QString str = trim(filename_);
	QTextStream out(file_);
	out.setCodec("UTF-8");
	out << str;
	file_->close();
	QDir dir(workdir_);
	if (dir.exists()) {
		qWarning() << uuid_ << "directory already exist.";
		return false;
	}
	else {
		if (!dir.mkdir(workdir_)) {
			qWarning() << uuid_ << ":can't create work directory.";
			return false;
		}
		else {
			return true;
		}
	}
}

bool InputDeck::runMelgen()
{
	thandle = OpenSharedMemory(uuid_.toLocal8Bit().data(), 2048);
	QProcess process;
	process.setWorkingDirectory(workdir_);
	qInfo() << uuid_ << ":start melgen.";
	process.start("./bin/Melgen.exe", QStringList() << "i=" + file_->fileName() << "s=" + uuid_);
	if (!process.waitForFinished(10000)) {
		qCritical() << uuid_ << ":melgen overtime.";
		return false;
	}
	process.close();
	return true;
}

bool InputDeck::runMelcor()
{
	connect(&process_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &InputDeck::processfinished);
	connect(&process_, &QProcess::errorOccurred, this, &InputDeck::processerrror);
	process_.setWorkingDirectory(workdir_);
	qInfo() << uuid_ << ":start melcor.";
	//process_.start("./bin/Melcor.exe", QStringList() << file_->fileName()<< QDir::currentPath() + "/bin/" << uuid_);
	process_.start("./bin/Melcor.exe", QStringList() << "i=" + file_->fileName() << "s=" + uuid_);
	return true;
}

void InputDeck::processfinished(int exitCode, const QProcess::ExitStatus &exitStatus)
{
	if (exitStatus == QProcess::CrashExit)
	{
		qCritical() << uuid_ << ":melcor crashed.";
	}
	else if (exitCode != 0)
	{
		qWarning() << uuid_ << ":melcor exited with code:" << exitCode;
	}
	else
	{
		qInfo() << uuid_ << ":melcor finished.";
	}
	thandle = 0;
	emit finished(uuid_);
}

void InputDeck::processerrror(const QProcess::ProcessError &error)
{
	if (error == QProcess::FailedToStart)
	{
		qCritical() << uuid_ << ":melcor not found.";
	}
	CloseSharedMemory(thandle);
	emit aborted(uuid_);
}