#pragma once
#include <QObject>
#include <QString>
#include <QTemporaryFile>
#include <QProcess>
#include <QTimer>

QString trim(const QString &str);
QString strippedName(const QString &fullFileName);

class InputDeck :
	public QObject
{
	Q_OBJECT
public:
	explicit InputDeck(const QString &uuid, const QString &filename,QObject *parent = nullptr);
	~InputDeck();
	static void check(const QString &filename);
	int Interval() const;
	void SetInterval(int k);
	QString WorkDir() const;
	void SetWorkDir(const QString &filepath);
	QString uuid() const;
	bool isRunning();
	void Run();
	void destroy();
	void showChart();

signals:
	void start(const QString &uuid);
	void aborted(const QString &uuid);
	void curprogress(const QString &uuid, const QString &str);
	void chartmessage(const QString &str);
	void finished(const QString &uuid);

private:
	int interval_;
	QString workdir_;
	QString uuid_;
	QString filename_;
	QTimer timer_;
	QTemporaryFile *file_;
	QProcess process_;
	long int thandle;

	void ReadOutput();
	bool init();
	bool runMelgen();
	bool runMelcor();
	void processfinished(int exitCode, const QProcess::ExitStatus &exitStatus);
	void processerrror(const QProcess::ProcessError &error);
};

