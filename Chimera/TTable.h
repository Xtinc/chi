#pragma once
#include <QTableWidget>

class InputDeck;

class TTable :
	public QTableWidget
{
	Q_OBJECT
public:
	explicit TTable(QWidget *parent = nullptr);
	~TTable();
	void addTask(const QString &filepath, const QString &workPath);
	bool isTaskRunning();
	void stopAllTask();

private:
	QMap<QString, InputDeck*> taskList;

	void stoptask(const QString &uuid);
	void showDig(const QString &uuid);
	void contextMenu(const QPoint &pos);
};

static inline QString uuidOfItem(const QTableWidgetItem *item)
{
	return item->data(Qt::UserRole + 1).toString();
}
