#pragma once
#include "ui_PlotDialog.h"
#include "GraphWidget.h"
#include <QDialog>

class PlotDialog :public QDialog
{
public:
	PlotDialog(const QStringList &list, QWidget *parent = nullptr);
	QString returnTitle();
	GraphWidget::ChartType returnType();
	QStringList returnXY();
private:
	Ui::PlotDialog ui;
};