#include "ContentWidget.h"
#include "Application.h"
#include "QtAwesome/QtAwesome.h"
#include "melcordig.h"
#include "Chimera.h"
#include "qtdiff/util/zfile.h"
#include <QStandardPaths>
#include <QDebug>


ContentWidget::ContentWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	strmodel = new QStringListModel(this);
	ui.VarView->setModel(strmodel);
	ui.VarView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui.pushButton_3->setIcon(myApp()->qtAwesome()->icon(fa::plus));
	ui.pushButton->setIcon(myApp()->qtAwesome()->icon(fa::close));
	connect(ui.pushButton, &QPushButton::pressed, this, &ContentWidget::onClearPressed);
	connect(ui.pushButton_3, &QPushButton::pressed, this, &ContentWidget::onNewTaskPressed);
	ui.listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	connect(ui.listWidget, &QListWidget::doubleClicked, this, &ContentWidget::onListViewClicked);
	updateModel();
}


ContentWidget::~ContentWidget()
{
}

void ContentWidget::RunTask(const QString & str, const QString & str2)
{
	if (str.isEmpty()) {
		qWarning() << "Invalid Input Deck.";
		return;
	}
	else
	{
		if (str2.isEmpty()) {
			ui.tableWidget->addTask(str, QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
		}
		else
		{
			ui.tableWidget->addTask(str, str2);
		}
	}
}

void ContentWidget::updateModel()
{
	ui.listWidget->clear();
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	QStringList list = readRecentFiles(settings);
	for (int i = 0; i < list.length(); ++i) {
		QListWidgetItem *item=new QListWidgetItem;
		item->setData(Qt::DisplayRole,strippedName(list.at(i)));
		item->setData(Qt::ToolTipRole, list.at(i));
		item->setIcon(ZFile::icon(list.at(i)));
		ui.listWidget->addItem(item);
	}
}

void ContentWidget::updateVars(const QStringList & list)
{
	strmodel->setStringList(list);
}

void ContentWidget::onNewTaskPressed()
{
	melcorDig dig(this);
	if (dig.exec() == QDialog::Accepted) {
		ui.tableWidget->addTask(dig.FileName(), dig.WorkPath());
	}
}

void ContentWidget::onClearPressed()
{
	emit RequireClearVar();
}

void ContentWidget::onListViewClicked(const QModelIndex &index)
{
	QVariant string = index.data(Qt::ToolTipRole);
	emit RequireOpenFile(string.toString());
}