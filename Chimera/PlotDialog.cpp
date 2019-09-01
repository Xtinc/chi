#include "PlotDialog.h"
#include <QStringListModel>

PlotDialog::PlotDialog(const QStringList &list, QWidget * parent)
	:QDialog(parent)
{
	ui.setupUi(this);
	QStringListModel *model = new QStringListModel(list);
	ui.ylistView->setModel(model);
	ui.ylistView->setSelectionMode(QAbstractItemView::ExtendedSelection);
	ui.ylistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.xcmb->setModel(model);
}

QString PlotDialog::returnTitle()
{
	return ui.lineEdit->text();
}

GraphWidget::ChartType PlotDialog::returnType()
{
	GraphWidget::ChartType type;
	if (ui.typcmb->currentText() == "Line Chart")
	{
		type = GraphWidget::ChartType::Line;
	}
	else if(ui.typcmb->currentText() == "Spline Chart")
	{
		type = GraphWidget::ChartType::Spline;
	}
	else
	{
		type = GraphWidget::ChartType::Scatter;
	}
	return type;
}

QStringList PlotDialog::returnXY()
{
	QStringList list;
	list.append(ui.xcmb->currentText());
	QItemSelectionModel *selmodel = ui.ylistView->selectionModel();
	QModelIndexList modelIndexList = selmodel->selectedIndexes();
	for (auto i :modelIndexList)
	{
		list.append(ui.ylistView->model()->data(i).toString());
	}
	return list;
}