#include "TTable.h"
#include "InputDeck.h"
#include "QSpreadsheetHeaderView.h"
#include <QUuid>
#include <QDir>
#include <QDateTime>
#include <QHeaderView>
#include <QMenu>
#include <QProgressBar>
#include <QGuiApplication>
#include <QDesktopServices>
#include <QUrl>
#ifndef QT_NO_CLIPBOARD
#include <QClipboard>
#endif


TTable::TTable(QWidget *parent)
	:QTableWidget(0, 7, parent)
{
	//setSelectionBehavior(QAbstractItemView::SelectRows);

	//QSpreadsheetHeaderView *hv = new QSpreadsheetHeaderView(Qt::Horizontal, this);
	//setHorizontalHeader(hv);
	//QStringList labels;
	//labels << tr("Input") << tr("WorkDir") << tr("Progress") << tr("TimeStart") << tr("TimeEnd") << tr("Status") << tr("UUID");
	//setHorizontalHeaderLabels(labels);
	//horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	//horizontalHeader()->setStretchLastSection(true);
	//horizontalHeader()->setSectionsClickable(false);
	horizontalHeader()->hide();
	verticalHeader()->hide();
	//setShowGrid(false);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &TTable::customContextMenuRequested, this, &TTable::contextMenu);
}


TTable::~TTable()
{
	stopAllTask();
	clearContents();
}

void TTable::addTask(const QString &filePath, const QString &workPath)
{
	QString uuid = QUuid::createUuid().toString();
	QString uname = QDir::toNativeSeparators(filePath);

	QTableWidgetItem *inputItem = new QTableWidgetItem(uname);
	inputItem->setData(Qt::UserRole + 1, QVariant(uuid));
	inputItem->setToolTip(uname);
	inputItem->setFlags(inputItem->flags() ^ Qt::ItemIsEditable);

	uname = QDir::toNativeSeparators(workPath);
	QTableWidgetItem *outputItem = new QTableWidgetItem(uname);
	outputItem->setData(Qt::UserRole + 1, QVariant(uuid));
	outputItem->setToolTip(QDir::toNativeSeparators(uname));
	outputItem->setFlags(outputItem->flags() ^ Qt::ItemIsEditable);

	QTableWidgetItem *starttime = new QTableWidgetItem("-");
	starttime->setData(Qt::UserRole + 1, QVariant(uuid));
	starttime->setFlags(starttime->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
	starttime->setTextAlignment(Qt::AlignCenter);

	QTableWidgetItem* endtime = new QTableWidgetItem("-");
	endtime->setData(Qt::UserRole + 1, QVariant(uuid));
	endtime->setFlags(endtime->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
	endtime->setTextAlignment(Qt::AlignCenter);

	QTableWidgetItem* statusitem = new QTableWidgetItem("-");
	statusitem->setData(Qt::UserRole + 1, QVariant(uuid));
	statusitem->setFlags(statusitem->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
	statusitem->setTextAlignment(Qt::AlignCenter);

	QTableWidgetItem* proitem = new QTableWidgetItem("-");
	proitem->setData(Qt::UserRole + 1, QVariant(uuid));
	proitem->setFlags(proitem->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
	proitem->setTextAlignment(Qt::AlignCenter);

	QTableWidgetItem* UUIDitem = new QTableWidgetItem(uuid);
	UUIDitem->setData(Qt::UserRole + 1, QVariant(uuid));
	UUIDitem->setFlags(UUIDitem->flags() ^ (Qt::ItemIsEditable|Qt::ItemIsSelectable));
	UUIDitem->setTextAlignment(Qt::AlignCenter);

	int row = rowCount();
	insertRow(row);
	setItem(row, 0, inputItem);
	setItem(row, 1, outputItem);
	setItem(row, 2, proitem);
	setItem(row, 3, starttime);
	setItem(row, 4, endtime);
	setItem(row, 5, statusitem);
	setItem(row, 6, UUIDitem);
	//

	InputDeck *newtask = new InputDeck(uuid, filePath);
	connect(newtask, &InputDeck::start, [starttime, statusitem] {
		starttime->setText(QDateTime::currentDateTime().toString("hh:mm:ss\nyyyy/MM/dd"));
		statusitem->setText("Running");
	});
	connect(newtask, &InputDeck::curprogress, [proitem](const QString &uuid, const QString &k) {
		proitem->setText(QString("%1%").arg(k.toDouble()));
	});
	connect(newtask, &InputDeck::aborted, [endtime, statusitem] {
		endtime->setText(QDateTime::currentDateTime().toString("hh:mm:ss\nyyyy/MM/dd"));
		statusitem->setText("Aborted");
	});
	connect(newtask, &InputDeck::finished, [endtime, statusitem] {
		endtime->setText(QDateTime::currentDateTime().toString("hh:mm:ss\nyyyy/MM/dd"));
		statusitem->setText("Finished");
	});
	newtask->SetWorkDir(workPath);
	newtask->Run();
	taskList.insert(uuid, newtask);
}

bool TTable::isTaskRunning()
{
	bool t = false;
	for (auto i : taskList) {
		t = t || i->isRunning();
	}
	return t;
}

void TTable::stopAllTask()
{
	for (auto i : taskList) {
		i->destroy();
	}
}

void TTable::stoptask(const QString &uuid)
{
	InputDeck* task = taskList.find(uuid).value();
	if (task != nullptr) {
		task->destroy();
	}
}

void TTable::showDig(const QString &uuid)
{
	InputDeck* task = taskList.find(uuid).value();
	if (task != nullptr) {
		task->showChart();
	}
}

void TTable::contextMenu(const QPoint &pos)
{
	const QTableWidgetItem *item = itemAt(pos);
	if (!item)
		return;
	QMenu menu;
#ifndef QT_NO_CLIPBOARD
	QAction *copyAction = menu.addAction("Copy Name");
	QAction *folderAction = menu.addAction("Open In Folder");
#endif
	QAction *showAction = menu.addAction("Show Chart");
	QAction *stopAction = menu.addAction("Stop");
	QAction *action = menu.exec(mapToGlobal(pos));
	if (!action)
		return;
	const QString uuid = uuidOfItem(item);
	if (action == stopAction)
		stoptask(uuid);
	else if (action == showAction)
		showDig(uuid);
#ifndef QT_NO_CLIPBOARD
	else if (action == copyAction)
		QGuiApplication::clipboard()->setText(QDir::toNativeSeparators(item->text()));
#endif
	else if (action == folderAction)
		QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::toNativeSeparators(item->text())));
}