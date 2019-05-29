#include "FindThread.h"
#include <QRegularExpression>
#include <QDebug>
#include <QDirIterator>
#include <QMimeDatabase>

FindThread::FindThread(QWidget *parent)
	:stopped(false)
{
	setData("", "", "", false);
	timer = new QTime();
	timer->start();
}


FindThread::~FindThread()
{
	stopped = true;
	if (!wait(10000)) {
		qWarning() << "Thread:" << QThread::currentThreadId << "crashed.";
		terminate();
	}
	qInfo() << "Find-"<<ftext<<"finished,takes"<< timer->elapsed() / 1000<<"s.";
	delete timer;
}

void FindThread::setData(const QString & text, const QString & path, const QString & name, bool isreg)
{
	ftext = text;
	fpath = path;
	if (name.isEmpty()) {
		filter = QStringList() << "*";
	}
	else {
		filter = name.split(",", QString::SkipEmptyParts);
	}
	isREG = isreg;
}

void FindThread::stop()
{
	stopped = true;
}

void FindThread::run()
{
	QString rpath = QDir::cleanPath(fpath);
	currentDir = QDir(rpath);
	if (!ftext.isEmpty()) {
		qInfo() << "Find-" << ftext << "started.";
		if (isREG) {
			findRecursion(QRegularExpression(ftext), rpath);
		}
		else {
			findRecursion(ftext, rpath);
		}
	}
}

void FindThread::findRecursion(const QString & text, const QString & path)
{
	if (stopped) {
		return;
	}
	QString rpath = QDir::cleanPath(path);
	QDirIterator it(rpath, filter, QDir::Files | QDir::NoSymLinks);
	QStringList files;
	while (it.hasNext()) {
		files << it.next();
	}
	files = findFiles(files, text);
	std::sort(files.begin(), files.end());
	showFiles(files);
	QDirIterator jt(rpath, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	while (jt.hasNext()) {
		findRecursion(text, jt.next());
	}
}

void FindThread::findRecursion(const QRegularExpression & text, const QString & path)
{
	if (stopped) {
		return;
	}
	QString rpath = QDir::cleanPath(path);
	QDirIterator it(rpath, filter, QDir::Files | QDir::NoSymLinks);
	QStringList files;
	while (it.hasNext()) {
		files << it.next();
	}
	files = findFiles(files, text);
	std::sort(files.begin(), files.end());
	showFiles(files);
	QDirIterator jt(rpath, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	while (jt.hasNext()) {
		findRecursion(text, jt.next());
	}
}

QStringList FindThread::findFiles(const QStringList & files, const QString & text)
{
	QMimeDatabase mimeDatabase;
	QStringList foundFiles;

	for (auto i : files) {
		if (stopped) { break; };
		const QMimeType mimeType = mimeDatabase.mimeTypeForFile(i);
		if (mimeType.isValid() && !mimeType.inherits(QStringLiteral("text/plain"))) {
			continue;
		}
		QFile file(i);
		if (file.open(QIODevice::ReadOnly)) {
			QTextStream in(&file);
			while (!in.atEnd()) {
				if (stopped) { break; };
				if (in.readLine().contains(text, Qt::CaseInsensitive)) {
					foundFiles << i;
					break;
				}
			}
		}
	}
	return foundFiles;
}

QStringList FindThread::findFiles(const QStringList & files, const QRegularExpression & text)
{
	QMimeDatabase mimeDatabase;
	QStringList foundFiles;

	for (auto i : files) {
		if (stopped) { break; };
		const QMimeType mimeType = mimeDatabase.mimeTypeForFile(i);
		if (mimeType.isValid() && !mimeType.inherits(QStringLiteral("text/plain"))) {
			continue;
		}
		QFile file(i);
		if (file.open(QIODevice::ReadOnly)) {
			QTextStream in(&file);
			while (!in.atEnd()) {
				if (stopped) { break; };
				if (in.readLine().contains(text)) {
					foundFiles << i;
					break;
				}
			}
		}
	}
	return foundFiles;
}

void FindThread::showFiles(const QStringList & paths)
{
	for (const QString &filePath : paths) {
		const QString relativePath = QDir::toNativeSeparators(currentDir.relativeFilePath(filePath));
		const quint64 size = QFileInfo(filePath).size();
		emit requireNewItem(filePath, relativePath, size);
	}
}
