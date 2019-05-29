#pragma once
#include <QThread>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QTime>

class FindThread : public QThread
{
	Q_OBJECT
public:
	FindThread(QWidget *parent = nullptr);
	~FindThread();
	void setData(const QString &text, const QString &path, const QString &name, bool isreg);
	void stop();
signals:
	void requireNewItem(const QString &filepath, const QString &relativePath, quint64 size);
protected:
	void run();
private:
	bool isREG;
	bool stopped;
	QDir currentDir;
	QStringList filter;
	QString ftext, fpath;
	QTime *timer;

	void findRecursion(const QString &text, const QString &path);
	void findRecursion(const QRegularExpression &text, const QString &path);
	QStringList findFiles(const QStringList &files, const QString &text);
	QStringList findFiles(const QStringList &files, const QRegularExpression &text);
	void showFiles(const QStringList &paths);
};
