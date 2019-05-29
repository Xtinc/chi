#pragma once
#include <QWidget>
#include <QListWidgetitem>
#include "ui_ContentWidget.h"

class ContentWidget :
	public QWidget
{
	Q_OBJECT
public:
	ContentWidget(QWidget *parent = nullptr);
	~ContentWidget();
	void RunTask(const QString &str, const QString &str2);
	void updateModel();
signals:
	void RequireOpenFile(const QString &filename);

private:
	Ui::ContentWidget ui;
	void onNewTaskPressed();
	void onListViewClicked(const QModelIndex &index);
};

