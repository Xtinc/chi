#include "FindWidget.h"
#include <QDir>
#include <QMenu>
#include <QClipboard>

int FindWidget::count = 0;

static inline QString fileNameOfItem(const QTableWidgetItem *item)
{
	return item->data(Qt::UserRole + 1).toString();
}

FindWidget::FindWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

	QStringList labels;
	labels << "Filename" << "Size";
	ui.tableWidget->setHorizontalHeaderLabels(labels);
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	ui.tableWidget->verticalHeader()->hide();
	ui.tableWidget->setShowGrid(false);

	ui.srclb->setText(_taskname);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &FindWidget::customContextMenuRequested, this, &FindWidget::contextMenu);
	connect(ui.tableWidget, &QTableWidget::cellActivated, this, &FindWidget::openFileOfItem);

	++count;
}

FindWidget::~FindWidget()
{
	emit quitthread();
	ui.tableWidget->clearContents();
}

QString FindWidget::taskname() const
{
	return _taskname;
}

void FindWidget::setTaskname(const QString & str)
{
	_taskname = str;
	ui.srclb->setText(_taskname);
}

void FindWidget::showFiles(const QString & filePath, const QString & relativePath, quint64 size)
{
	const QString toolTip = QDir::toNativeSeparators(filePath);
	QTableWidgetItem *fileNameItem = new QTableWidgetItem(relativePath);
	fileNameItem->setData(Qt::UserRole + 1, QVariant(filePath));
	fileNameItem->setToolTip(toolTip);
	fileNameItem->setFlags(fileNameItem->flags() ^ Qt::ItemIsEditable);
	QTableWidgetItem *sizeItem = new QTableWidgetItem(QLocale().formattedDataSize(size));
	sizeItem->setData(Qt::UserRole + 1, QVariant(filePath));
	sizeItem->setToolTip(toolTip);
	sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

	int row = ui.tableWidget->rowCount();
	ui.tableWidget->insertRow(row);
	ui.tableWidget->setItem(row, 0, fileNameItem);
	ui.tableWidget->setItem(row, 1, sizeItem);
}

void FindWidget::openFileOfItem(int row, int column)
{
	const QTableWidgetItem *item = ui.tableWidget->item(row, 0);
	emit requireOpenfile(fileNameOfItem(item));
}

void FindWidget::contextMenu(const QPoint & pos)
{
	const QTableWidgetItem *item = ui.tableWidget->itemAt(pos);
	if (!item)
		return;
	QMenu menu;
#ifndef QT_NO_CLIPBOARD
	QAction *copyAction = menu.addAction("Copy Name");
#endif
	QAction *openAction = menu.addAction("Open");
	QAction *action = menu.exec(parentWidget()->mapToGlobal(pos));
	if (!action)
		return;
	const QString fileName = fileNameOfItem(item);
	if (action == openAction)
		emit requireOpenfile(fileName);
#ifndef QT_NO_CLIPBOARD
	else if (action == copyAction)
		QGuiApplication::clipboard()->setText(QDir::toNativeSeparators(fileName));
#endif
}
