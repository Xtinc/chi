#pragma once
#include <QWidget>
#include "ui_FindWidget.h"

class FindWidget :
	public QWidget
{
	Q_OBJECT
public:
	static int count;
public:
	FindWidget(QWidget *parent = nullptr);
	~FindWidget();
	QString taskname()const;
	void setTaskname(const QString &str);
	void showFiles(const QString &filepath, const QString &relativePath, quint64 size);
signals:
	void requireOpenfile(const QString &name);
	void quitthread();

private:
	Ui::FindWidget ui;
	QString _taskname;

private:
	void openFileOfItem(int row, int column);
	void contextMenu(const QPoint &pos);
};

