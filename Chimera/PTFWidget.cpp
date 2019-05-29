#include "PTFWidget.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QMenu>

int PTFWidget::count = 0;

PTFWidget::PTFWidget()
	:isFold(true)
{
	ui.setupUi(this);
	ui.treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.treeWidget->header()->setDefaultAlignment(Qt::AlignCenter);
	ui.treeWidget_2->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.treeWidget_2->header()->setDefaultAlignment(Qt::AlignCenter);
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &PTFWidget::customContextMenuRequested, this, &PTFWidget::ShowMenu);
	++count;
}

PTFWidget::~PTFWidget()
{
}

QString PTFWidget::taskname() const
{
	return filename;
}

QString PTFWidget::PTFfileName() const
{
	return filename;
}

void PTFWidget::setFile(const QString & name)
{
	filename = name;
	SpecifiedList();
	ui.dstlabel->setText(QStringLiteral("variables:%1  dimensions:%2").arg(nkeyt).arg(nrect));
}

void PTFWidget::SpecifiedList()
{
	PTFile ptfile(filename);
	nkeyt = ptfile.Getnkeyt();
	nrect = ptfile.Getnrect();
	pdlist = ptfile.GetPVNames();
	QStringList dimlist = ptfile.GetPVDimensions();
	idxlist = ptfile.GetPlotID();
	QVector<int> idxname = ptfile.GetPlotND();
	QStringList infolist = ptfile.GetPVInfo();
	for (int i = 0; i < pdlist.length(); i++)
	{
		QString str = pdlist.at(i);
		if (str.contains("-")) {
			QString suffix = str.split("-").first();
			QList<QTreeWidgetItem*> itemlist = ui.treeWidget->findItems(suffix, nullptr);
			QTreeWidgetItem *pitem;
			if (itemlist.isEmpty()) {
				pitem = new QTreeWidgetItem();
				pitem->setText(0, suffix);
				ui.treeWidget->addTopLevelItem(pitem);
			}
			else {
				pitem = itemlist.first();
			}
			QTreeWidgetItem *citem = new QTreeWidgetItem();
			citem->setText(0, str);
			citem->setTextAlignment(1, Qt::AlignCenter);
			QString dimension = dimlist.at(i);
			if (dimension.isEmpty()) {
				citem->setText(1, "-");
			}
			else {
				citem->setText(1, dimension);
			}
			pitem->addChild(citem);
			if (i < pdlist.length() - 1) {
				int idxstart = idxlist.at(i) - 1;
				int idxend = idxlist.at(i + 1) - 1;
				for (int j = idxstart; j < idxend; ++j) {
					if (idxname.at(j) != 0) {
						QTreeWidgetItem *titem = new QTreeWidgetItem();
						titem->setText(0, QString::number(idxname.at(j)));
						citem->addChild(titem);
					}
				}
			}
			else {
				for (int j = idxlist.at(i) - 1; j < idxname.length(); ++j) {
					if (idxname.at(j) != 0) {
						QTreeWidgetItem *titem = new QTreeWidgetItem();
						titem->setText(0, QString::number(idxname.at(j)));
						citem->addChild(titem);
					}
				}
			}
		}
		else {
			QTreeWidgetItem *pitem = new QTreeWidgetItem();
			pitem->setText(0, str);
			pitem->setFlags(pitem->flags()&(~Qt::ItemIsSelectable));
			ui.treeWidget->addTopLevelItem(pitem);
		}
	}
	for (int k = 0; k < infolist.length(); ++k) {
		QString str = infolist.at(k).split("((").first();
		QList<QTreeWidgetItem*> itemlist = ui.treeWidget_2->findItems(str, nullptr);
		QTreeWidgetItem *pitem;
		if (itemlist.isEmpty()) {
			pitem = new QTreeWidgetItem();
			pitem->setText(0, str);
			ui.treeWidget_2->addTopLevelItem(pitem);
		}
		else {
			pitem = itemlist.first();
		}
		QTreeWidgetItem *citem = new QTreeWidgetItem();
		citem->setTextAlignment(1, Qt::AlignCenter);
		QRegularExpression reg("\\(\\((\\d+)\\)\\)(\\S+)");
		QRegularExpressionMatch match = reg.match(infolist.at(k));
		if (match.hasMatch()) {
			citem->setText(0, match.captured(1));
			bool tr;
			QString str = match.captured(2);
			double t = match.captured(2).toDouble(&tr);
			if (tr) {
				citem->setText(1,QString::number(t));
			}
			else
			{
				citem->setText(1, match.captured(2));
			}
		}
		pitem->addChild(citem);
	}/*
	QFileInfo info(QDir::currentPath() + QDir::separator() + "PTFReaderA.exe");
	if (info.exists()) {
		process.start(QDir::currentPath()+QDir::separator()+"PTFReaderA.exe", QStringList() << filename);
		connect(&process, &QProcess::readyReadStandardOutput, this, &PTFWidget::showmessage);
		process.waitForFinished();
		QStringList list = content.split("&&");
		Q_ASSERT(list.length() == 8);
		QString title = list.at(0).simplified();
		nkeyt = list.at(1).toInt();
		nrect = list.at(2).toInt();
		QStringList pdlist = list.at(3).simplified().split(" ");
		QString dim = list.at(4);
		dim.remove("\r\n ");
		QString idxstr = list.at(5);
		QStringList idxlist = idxstr.remove("\r\n ").simplified().split(" ", QString::SkipEmptyParts);
		idxstr = list.at(6);
		QStringList idxname = idxstr.remove("\r\n ").simplified().split(" ", QString::SkipEmptyParts);
		idxstr = list.at(7);
		QStringList infolist = idxstr.remove("\r\n ").simplified().split(".SP/", QString::SkipEmptyParts);
		for (int i = 0; i < pdlist.length(); ++i) {
			QString str = pdlist.at(i);
			if (str.contains("-")) {
				QString suffix = str.split("-").first();
				QList<QTreeWidgetItem*> itemlist = ui.treeWidget->findItems(suffix, nullptr);
				QTreeWidgetItem *pitem;
				if (itemlist.isEmpty()) {
					pitem = new QTreeWidgetItem();
					pitem->setText(0, suffix);
					ui.treeWidget->addTopLevelItem(pitem);
				}
				else {
					pitem = itemlist.first();
				}
				QTreeWidgetItem *citem = new QTreeWidgetItem();
				citem->setText(0, str);
				citem->setTextAlignment(1, Qt::AlignCenter);
				QString dimension = dim.mid(16 * i - 1, 15).simplified();
				if (dimension.isEmpty()) {
					citem->setText(1, "-");
				}
				else {
					citem->setText(1, dimension);
				}
				pitem->addChild(citem);
				if (i < pdlist.length() - 1) {
					int idxstart = idxlist.at(i).toInt() - 1;
					int idxend = idxlist.at(i + 1).toInt() - 1;
					for (int j = idxstart; j < idxend; ++j) {
						if (idxname.at(j).toInt() != 0) {
							QTreeWidgetItem *titem = new QTreeWidgetItem();
							titem->setText(0, idxname.at(j));
							citem->addChild(titem);
						}
					}
				}
				else {
					for (int j = idxlist.at(i).toInt() - 1; j < idxname.length(); ++j) {
						if (idxname.at(j).toInt() != 0) {
							QTreeWidgetItem *titem = new QTreeWidgetItem();
							titem->setText(0, idxname.at(j));
							citem->addChild(titem);
						}
					}
				}
			}
			else {
				QTreeWidgetItem *pitem = new QTreeWidgetItem();
				pitem->setText(0, str);
				ui.treeWidget->addTopLevelItem(pitem);
			}
		}
		for (int k = 0; k < infolist.length(); ++k) {
			QString str = infolist.at(k).split("((").first();
			QList<QTreeWidgetItem*> itemlist = ui.treeWidget_2->findItems(str, nullptr);
			QTreeWidgetItem *pitem;
			if (itemlist.isEmpty()) {
				pitem = new QTreeWidgetItem();
				pitem->setText(0, str);
				ui.treeWidget_2->addTopLevelItem(pitem);
			}
			else {
				pitem = itemlist.first();
			}
			QTreeWidgetItem *citem = new QTreeWidgetItem();
			citem->setTextAlignment(1, Qt::AlignCenter);
			QRegularExpression reg("\\(\\((\\d+)\\)\\)(\\S+)");
			QRegularExpressionMatch match = reg.match(infolist.at(k));
			if (match.hasMatch()) {
				citem->setText(0, match.captured(1));
				citem->setText(1, match.captured(2));
			}
			pitem->addChild(citem);
		}
	}
	else
	{
		qWarning() << "PTFReaderA.exe not exists.";
	}*/
}

void PTFWidget::ShowMenu(const QPoint &pos)
{
	int lxb = ui.treeWidget->x();
	int rxb = lxb + ui.treeWidget->width();
	int tyb = ui.treeWidget->y();
	int byb = tyb + ui.treeWidget->height();
	if (pos.x() > lxb&&pos.x() < rxb&&pos.y() > tyb&&pos.y() < byb) {
		QMenu menu;
		QAction *PlotAct = new QAction("Plot Selected Variables", &menu);
		connect(PlotAct, &QAction::triggered, this, &PTFWidget::PlotVars);
		QAction *foldAct = new QAction("Fold/Unfold", &menu);
		connect(foldAct, &QAction::triggered,this,&PTFWidget::FoldOrExpand);
		menu.addAction(PlotAct);
		menu.addAction(foldAct);
		menu.exec(mapToGlobal(pos));
	}
}

void PTFWidget::PlotVars()
{
	QVector<int> idx;
	QStringList namelist;
	for (auto i : ui.treeWidget->selectedItems()) {
		if (i->parent() == nullptr) {
			return;
		}
		else if (i->parent()->parent() == nullptr) {
			int ix = pdlist.indexOf(QRegularExpression(i->text(0)));
			if (ix != -1) {
				int rix = idxlist.at(ix);
				idx.append(rix);
				namelist.append(i->text(0).simplified());
			}
		}
		else if (i->parent()->parent()->parent() == nullptr) {
			int ix = pdlist.indexOf(QRegularExpression(i->parent()->text(0)));
			int k = i->parent()->indexOfChild(i);
			int rix = idxlist.at(ix) + k;
			idx.append(rix);
			namelist.append(i->parent()->text(0).simplified() + "." + i->text(0).simplified());
		}
		else
		{
			return;
		}
		/*if (i->childCount()== 0) {
			int ix = pdlist.indexOf(QRegularExpression(i->parent()->text(0)));
			int k = i->parent()->indexOfChild(i);
			int rix = idxlist.at(ix) + k;
			idx.append(rix);
			namelist.append(i->parent()->text(0).simplified() +"."+ i->text(0).simplified());
		}
		else
		{
			
		}*/
	}
	if (!idx.isEmpty()) {
		emit RequirePLOT(filename,namelist, idx);
	}
}

void PTFWidget::FoldOrExpand()
{
	if (isFold) {
		ui.treeWidget->expandAll();
	}
	else
	{
		ui.treeWidget->collapseAll();
	}
	isFold = !isFold;
}

PTFile::PTFile(const QString & _filename)
{
	filename = _filename;
	QFile inFile(filename);
	if (!inFile.open(QIODevice::ReadOnly)) {
		qWarning() << "Can not open " << filename.toLatin1().data();
	}
	QDataStream stream(&inFile);
	init(stream);
	inFile.close();
}

QList<QVector<float>> PTFile::GetPlot(const QVector<int>& idx)
{
	QFile inFile(filename);
	if (!inFile.open(QIODevice::ReadOnly)) {
		qWarning() << "Can not open " << filename.toLatin1().data();
	}
	QDataStream stream(&inFile);
	init(stream);
	bool t = true;
	char *buf;
	uint strLen;
	qint32 offset;
	float stime,sdt,scpu,ncycle,var;
	QList<QVector<float>> plotdata;
	QVector<float> xdata[24];
	
	stream.skipRawData(4);
	stream.readRawData((char*)&stime, sizeof(float));
	stream.skipRawData(12);
	xdata[0].append(stime);
	/*stream.readRawData((char*)&sdt, sizeof(float));
	stream.readRawData((char*)&scpu, sizeof(float));
	stream.readRawData((char*)&ncycle, sizeof(float));;
	xdata[1].append(sdt);
	xdata[2].append(scpu);
	xdata[3].append(ncycle);*/
	for (int i = 0; i < qMin(22, idx.length()); i++)
	{
		int skip;
		if (i > 0) {
			skip = 4 * (idx.at(i) - idx.at(i - 1) - 1);
		}
		else {
			skip = 4 * (idx.at(i) - 1);
		}
		stream.skipRawData(skip);
		stream.readRawData((char*)&var, sizeof(float));
		xdata[i + 1].append(var);
	}
	stream.skipRawData(4 * (1 + nrect - idx.at(qMin(22, idx.length() - 1))));
	while (t)
	{
		stream.readBytes(buf, strLen);
		if (buf == NULL) {
			break;
		}
		if (strcmp(buf, ".TR/") == 0) {
			stream.skipRawData(4+4);
			stream.readRawData((char*)&stime, sizeof(float));
			stream.skipRawData(12);
			xdata[0].append(stime);
			for (int i = 0; i < qMin(22,idx.length()); i++)
			{
				int skip;
				if (i > 0) {
					skip = 4 * (idx.at(i) - idx.at(i - 1) - 1);
				}
				else {
					skip = 4*(idx.at(i)-1);
				}
				stream.skipRawData(skip);
				stream.readRawData((char*)&var, sizeof(float));
				xdata[i+1].append(var);
			}
			stream.skipRawData(4 * (1 + nrect - idx.at(qMin(22, idx.length() - 1))));
		}
		else
		{
			t = false;
		}
	}
	inFile.close();
	for (auto i : xdata) {
		if (!i.isEmpty()) {
			plotdata.append(i);
		}
	}
	return plotdata;
}

void PTFile::init(QDataStream &stream)
{
	stream.setByteOrder(QDataStream::LittleEndian);
	char *buf;
	char name[25] = { 0 };
	uint strLen;
	qint32 offset;
	auto ReadForStr = [&stream](char *&buf, uint &len) {
		stream.readBytes(buf, len);
		stream.skipRawData(4);
		//in Fortran write function ,it will write strlen+str+strlen in binary
		//other than strlen+str in C. so it's necessary to skip the last strlen. 
	};
	stream.skipRawData(12 + 12);
	ReadForStr(buf, strLen);
	title = QLatin1String(buf);
	stream.skipRawData(12 + 12 + 4);
	stream.readRawData((char*)&nkeyt, sizeof(qint32));
	stream.readRawData((char*)&nrect, sizeof(qint32));
	stream.skipRawData(8);
	for (int i = 0; i < nkeyt; i++)
	{
		stream.readRawData((char*)&name, 24);
		PVNames.append(QLatin1String(name));
	}
	stream.skipRawData(8);
	for (int i = 0; i < nkeyt; i++)
	{
		stream.readRawData((char*)&offset, sizeof(qint32));
		PlotID.append(offset);
	}
	stream.skipRawData(8);
	for (int i = 0; i < nkeyt; i++)
	{
		stream.readRawData((char*)&name, 16);
		PVDimensions.append(QLatin1String(name));
	}
	stream.skipRawData(8);
	for (int i = 0; i < nrect; i++)
	{
		stream.readRawData((char*)&offset, sizeof(qint32));
		PlotND.append(offset);
	}
	stream.skipRawData(4);
	bool t = true;
	while (t)
	{
		ReadForStr(buf, strLen);
		if (strcmp(buf, ".SP/") == 0) {
			ReadForStr(buf, strLen);
			PVinfo.append(QLatin1String(buf));
		}
		else
		{
			t = false;
		}
	}
}
