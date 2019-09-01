#pragma once
#include <QWidget>
#include <QListWidgetitem>
#include <QStringListModel>
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
	void updateVars(const QStringList &list);
signals:
	void RequireOpenFile(const QString &filename);
	void RequireClearVar();
private:
	Ui::ContentWidget ui;
	QStringListModel *strmodel;
	void onNewTaskPressed();
	void onClearPressed();
	void onListViewClicked(const QModelIndex &index);
};

