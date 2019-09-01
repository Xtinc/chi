#include "Chimera.h"
#include "Highlighter.h"
#include "RegularExpressionDialog.h"
#include "FindThread.h"
#include "ContentWidget.h"
#include "diffdialog.h"
#include "ads/API.h"
#include "ads/ContainerWidget.h"
#include "ads/SectionWidget.h"
#include "QtAwesome/QtAwesome.h"
#include "qtdiff/widget/folder/zfolderwidget.h"
#include "qtdiff/widget/file/zfilewidget.h"
#include "VarPool.h"
#include "PlotDialog.h"
#include <QLabel>
#include <QDebug>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>
#include <QVector>

Chimera::Chimera(QWidget *parent)
	: QMainWindow(parent),_sectionwidget(nullptr)
{
	varpool = new VarPool;
	_container = new ADS_NS::ContainerWidget;
	setCentralWidget(_container);
	ConstructContent();
	ConstructConsole();
	ConstructMenu();
	setAcceptDrops(true);
	resize(2 * myApp()->desktop()->screenGeometry().width() / 3, 2 * myApp()->desktop()->screenGeometry().height() / 3);
}

Chimera::~Chimera()
{
	delete varpool;
}

QPlainTextEdit * Chimera::console() const
{
	return _console;
}

void Chimera::closeEvent(QCloseEvent * e)
{
	bool t = true;
	for (auto i : _container->contents()) {
		if (i!=nullptr&&i->uniqueName().contains("File", Qt::CaseInsensitive)) {
			Editor *edit = static_cast<Editor *>(i->contentWidget());
			t = t && edit->maybeSave();
		}
	}
	if (t) {
		e->accept();
	}
	else
	{
		e->ignore();
	}
}

void Chimera::dragEnterEvent(QDragEnterEvent * event)
{
	event->accept();
}

void Chimera::dropEvent(QDropEvent * event)
{
	const QMimeData *data = event->mimeData();
	if (data->hasUrls()) {
		QList<QUrl> list = data->urls();
		Chimera::OpenFile(list.first().toLocalFile());
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void Chimera::ConstructConsole()
{
	_console = new QPlainTextEdit;
	Highlighter *highlighter = new Highlighter(_console->document());
	_console->setReadOnly(true);
	_console->setFrameShape(QFrame::NoFrame);
	IconTitleWidget *title = new IconTitleWidget(QIcon(myApp()->qtAwesome()->icon(fa::terminal)),"Log");
	ADS_NS::SectionContent::RefPtr SC = ADS_NS::SectionContent::newSectionContent(QString("Console"), _container,title, _console);
	SC->setTitle("Log");
	_container->addSectionContent(SC, _sectionwidget,ADS_NS::BottomDropArea);
}

void Chimera::ConstructMenu()
{
	QMenu *fileMenu = menuBar()->addMenu("File");

	QAction *openAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::folderopeno)), "Open...", this);
	connect(openAct, &QAction::triggered, this, &Chimera::Open);
	fileMenu->addAction(openAct);

	QAction *newtextfileAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::filecodeo)),"New File",this);
	connect(newtextfileAct, &QAction::triggered, this, &Chimera::newTextFile);
	fileMenu->addAction(newtextfileAct);

	QAction *newplotfileAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::image)), "New Plot", this);
	connect(newplotfileAct, &QAction::triggered, this, &Chimera::newPlotFile);
	fileMenu->addAction(newplotfileAct);

	fileMenu->addSeparator();

	recentMenu = fileMenu->addMenu(tr("Recent..."));
	connect(recentMenu, &QMenu::aboutToShow, this, &Chimera::updateRecentFileActions);
	recentFileSubMenuAct = recentMenu->menuAction();

	for (int i = 0; i < MaxRecentFiles; ++i) {
		recentFileActs[i] = recentMenu->addAction(QString(), this, &Chimera::openRecentFile);
		recentFileActs[i]->setVisible(false);
	}

	recentFileSeparator = fileMenu->addSeparator();

	setRecentFilesVisible(hasRecentFiles());

	viewMenu = menuBar()->addMenu("View");
	connect(viewMenu, &QMenu::aboutToShow, this, &Chimera::updateWindowMenu);

	fileMenu->addSeparator();

	QAction *closehiddenAct = new QAction("Close Hidden Windows", this);
	connect(closehiddenAct, &QAction::triggered, this, &Chimera::closeHidden);
	fileMenu->addAction(closehiddenAct);

	QAction *closeAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::close)), "Close All", this);
	connect(closeAct, &QAction::triggered, this, &Chimera::closeAll);
	fileMenu->addAction(closeAct);

	fileMenu->addSeparator();

	QMenu *diffMenu = menuBar()->addMenu("Find&&Diff");
	QAction* findinfloderAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::search)), "Find In Folder", this);
	connect(findinfloderAct, &QAction::triggered, this, &Chimera::showFindDig);
	diffMenu->addAction(findinfloderAct);

	QAction* diffAct = new QAction(QIcon(), "Start Compare", this);
	connect(diffAct, &QAction::triggered, this, &Chimera::showDiffDig);
	diffMenu->addAction(diffAct);

	QMenu *termineMenu = menuBar()->addMenu("Terminal");
	tworkdir = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	QAction* settingsAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::gear)), "Working Folder", this);
	connect(settingsAct, &QAction::triggered, this, &Chimera::setTworkdir);
	termineMenu->addAction(settingsAct);
	QAction* MelgenAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::terminal)), "Melgen Terminal", this);
	connect(MelgenAct, &QAction::triggered, this, &Chimera::StartMelgenTerminal);
	termineMenu->addAction(MelgenAct);
	QAction* MelcorAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::terminal)), "Melcor Terminal", this);
	connect(MelcorAct, &QAction::triggered, this, &Chimera::StartMelcorTermial);
	termineMenu->addAction(MelcorAct);
}

void Chimera::ConstructContent()
{
	contentwidget = new ContentWidget;
	contentwidget->updateVars(varpool->GetNameList());
	ADS_NS::SectionContent::RefPtr SC = ADS_NS::SectionContent::newSectionContent("Start", _container,new QLabel("Start"),contentwidget);
	SC->setPinned(true);
	SC->setFlags(ADS_NS::SectionContent::None);
	_sectionwidget=_container->addSectionContent(SC);
	connect(contentwidget,&ContentWidget::RequireOpenFile,this,&Chimera::OpenFile);
	connect(contentwidget, &ContentWidget::RequireClearVar, this, &Chimera::ClearVars);
}

void Chimera::updateWindowMenu()
{
	viewMenu->clear();
	auto contents = _container->contents();
	for (auto i : contents) {
		if (!i->isPinned()) {
			QAction *a = new QAction(i->title(), viewMenu);
			a->setCheckable(true);
			a->setChecked(_container->isSectionContentVisible(i));
			a->setEnabled(i.data()->flags().testFlag(ADS_NS::SectionContent::Closeable));
			connect(a, &QAction::toggled, [this, i](bool t) {
				t ? this->_container->showSectionContent(i) : this->_container->hideSectionContent(i);
			});
			viewMenu->addAction(a);
		}
	}
}

void Chimera::newTextFile()
{
	Editor *edit = new Editor;
	ADS_NS::SectionContent::RefPtr SC = ConstructWin<Editor*>(edit,_container);
	_container->addSectionContent(SC, _sectionwidget, ADS_NS::CenterDropArea);
	_container->raiseSectionContent(SC);
	connect(edit, &Editor::titlechanged, this, &Chimera::prependToRecentFiles);
}

void Chimera::newPlotFile()
{
	PlotDialog dig(varpool->GetNameList(),this);
	if (dig.exec() == QDialog::Accepted) {
		QStringList tlist = dig.returnXY();
		QString x = tlist.first();
		tlist.removeFirst();
		QList<QVector<double>> tmpvec;
		for (int i = 0; i < tlist.length(); i++)
		{
			tmpvec.append(varpool->FindByName(tlist.at(i)));
		}
		GraphWidget *Graph = new GraphWidget;
		ADS_NS::SectionContent::RefPtr SC = ConstructWin<GraphWidget*>(Graph, _container);
		_container->addSectionContent(SC, _sectionwidget, ADS_NS::CenterDropArea);
		_container->raiseSectionContent(SC);
		Graph->setData(dig.returnTitle(), x, varpool->FindByName(x),
			tlist, tmpvec, dig.returnType());
	}
}

void Chimera::Open()
{
	const QString filename = QFileDialog::getOpenFileName(this);
	OpenFile(filename);
}

void Chimera::OpenFile(const QString &filename)
{
	if (!filename.isEmpty()) {
		for (auto i : _container->contents()) {
			if (i.data()->title() == filename) {
				_container->raiseSectionContent(i);
				return;
			}
		}
		QFileInfo info(filename);
		if (!info.exists()) {
			QMessageBox::StandardButton ret=
			QMessageBox::warning(this,"Chimera",QStringLiteral("Cannot read file :\n%1.\nDo you like to remove it from list?").arg(filename)
			,QMessageBox::Ok|QMessageBox::Cancel);
			if (ret = QMessageBox::Ok) {
				removeInvalidFiles(filename);
			}
			return;
		}
		if (QString::compare(info.suffix(), "ptf", Qt::CaseInsensitive) == 0) {
			PTFWidget *edit = new PTFWidget;
			edit->setFile(filename);
			connect(edit, &PTFWidget::RequirePLOT, this, &Chimera::CreatePTFPlot);
			connect(edit, &PTFWidget::RequireSAVE, this, &Chimera::updateVarList);
			auto con = ConstructWin<PTFWidget*>(edit, _container);
			_container->addSectionContent(con, _sectionwidget, ADS_NS::CenterDropArea);
			_container->raiseSectionContent(con);
		}
		else
		{
			Editor *edit = new Editor;
			edit->setFile(filename);
			connect(edit, &Editor::RequireRunCur, contentwidget, &ContentWidget::RunTask);
			auto con = ConstructWin<Editor*>(edit,_container);
			_container->addSectionContent(con,_sectionwidget,ADS_NS::CenterDropArea);
			_container->raiseSectionContent(con);
		}
		prependToRecentFiles(filename);
	}
}

void Chimera::closeAll()
{
	for (auto  i : _container->contents())
	{
		if (i->uniqueName().contains(QRegularExpression("File|Plot|Find|Plot|Graph|Diff"))) {
			_container->removeSectionContent(i);
		}
	}
}

void Chimera::closeHidden()
{
	for (auto i : _container->contents())
	{
		if (i->uniqueName().contains(QRegularExpression("File|Plot|Find|Plot|Graph|Diff"))&&(!_container->isSectionContentVisible(i))) {
			_container->removeSectionContent(i);
		}
	}
}

bool Chimera::hasRecentFiles()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	const int count = settings.beginReadArray(recentFilesKey());
	settings.endArray();
	return count > 0;
}

void Chimera::prependToRecentFiles(const QString & fileName)
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

	const QStringList oldRecentFiles = readRecentFiles(settings);
	QStringList recentFiles = oldRecentFiles;
	recentFiles.removeAll(fileName);
	recentFiles.prepend(fileName);
	if (oldRecentFiles != recentFiles)
		writeRecentFiles(recentFiles, settings);

	setRecentFilesVisible(!recentFiles.isEmpty());
	contentwidget->updateModel();
}

void Chimera::removeInvalidFiles(const QString & fileName)
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

	const QStringList oldRecentFiles = readRecentFiles(settings);
	QStringList recentFiles = oldRecentFiles;
	recentFiles.removeAll(fileName);
	if (oldRecentFiles != recentFiles)
		writeRecentFiles(recentFiles, settings);

	setRecentFilesVisible(!recentFiles.isEmpty());
	contentwidget->updateModel();
}

void Chimera::setRecentFilesVisible(bool visible)
{
	recentFileSubMenuAct->setVisible(visible);
	recentFileSeparator->setVisible(visible);
}

void Chimera::updateRecentFileActions()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());

	const QStringList recentFiles = readRecentFiles(settings);
	const int count = qMin(int(MaxRecentFiles), recentFiles.size());
	int i = 0;
	for (; i < count; ++i) {
		const QString fileName = QFileInfo(recentFiles.at(i)).fileName();
		recentFileActs[i]->setText(tr("&%1 %2").arg(i + 1).arg(fileName));
		recentFileActs[i]->setData(recentFiles.at(i));
		recentFileActs[i]->setVisible(true);
	}
	for (; i < MaxRecentFiles; ++i)
		recentFileActs[i]->setVisible(false);
}

void Chimera::openRecentFile()
{
	if (const QAction *action = qobject_cast<const QAction *>(sender()))
		OpenFile(action->data().toString());
}

void Chimera::showFindDig()
{
	RegularExpressionDialog dig(this);
	if (dig.exec() == QDialog::Accepted) {
		FindWidget *widget = new FindWidget;
		widget->setTaskname(dig.text());
		connect(widget, &FindWidget::requireOpenfile, this, &Chimera::OpenFile);
		FindThread *findthread = new FindThread;
		connect(widget, &FindWidget::quitthread, [findthread] {
			findthread->stop();
		});
		connect(findthread, &FindThread::finished, findthread, &FindThread::deleteLater);
		connect(findthread, &FindThread::requireNewItem, widget, &FindWidget::showFiles);
		findthread->setData(dig.text(), dig.dir(), dig.filter(), dig.isREG());
		findthread->start();

		ADS_NS::SectionContent::RefPtr SC = ConstructWin<FindWidget*>(widget, _container);
		_container->addSectionContent(SC, _sectionwidget, ADS_NS::CenterDropArea);
		_container->raiseSectionContent(SC);
	}
}

void Chimera::showDiffDig()
{
	DiffDialog dig(this);
	if (dig.exec() == QDialog::Accepted) {
		if (dig.isFile()) {
			ZPathDiffModel pathDiffModel;
			pathDiffModel.setSrcFileInfo(QFileInfo(dig.ReturnSrc()));
			pathDiffModel.setDstFileInfo(QFileInfo(dig.ReturnDst()));
			pathDiffModel.setStatus(Same);
			fileCompare(pathDiffModel);
		}
		else {
			ZFolderWidget *folderWidget = new ZFolderWidget;
			connect(folderWidget, &ZFolderWidget::fileCompare, this, &Chimera::fileCompare);
			ADS_NS::SectionContent::RefPtr SC = ConstructWin<ZFolderWidget*>(folderWidget, _container);
			_container->addSectionContent(SC, _sectionwidget, ADS_NS::CenterDropArea);
			_container->raiseSectionContent(SC);
			folderWidget->compare(dig.ReturnSrc(), dig.ReturnDst(), dig.ReturnFliter());
		}
	}
	else {
		return;
	}
}

void Chimera::fileCompare(const ZPathDiffModel & pathDiffModel)
{
	ZFileWidget *fileWidget = new ZFileWidget(pathDiffModel);
	QString srcFileName = pathDiffModel.srcFileInfo().fileName();
	QString dstFileName = pathDiffModel.dstFileInfo().fileName();
	if (srcFileName.isEmpty() && dstFileName.isEmpty())
	{
		qWarning() << "empty file name.";
		return;
	}
	else
	{
		fileWidget->compare();
	}
}

void Chimera::CreatePTFPlot(QString filepath, const QStringList &name, const QVector<int> &str)
{
	GraphWidget *Graph = new GraphWidget;
	ADS_NS::SectionContent::RefPtr SC = ConstructWin<GraphWidget*>(Graph, _container);
	_container->addSectionContent(SC, _sectionwidget, ADS_NS::CenterDropArea);
	_container->raiseSectionContent(SC);
	Graph->setPTF(filepath,name,str);
}

void Chimera::setTworkdir()
{
	QString directory = QFileDialog::getExistingDirectory(this, "Find files in...", ".");
	if (!directory.isEmpty()) {
		tworkdir = directory;
	}
}

void Chimera::StartMelgenTerminal()
{
	std::string appPath((QDir::currentPath() + "/bin/Melgen.exe").toLocal8Bit());
	std::string workPath(tworkdir.toLocal8Bit());
	int l = MultiByteToWideChar(CP_ACP, 0, appPath.c_str(), -1, NULL, 0);
	int m= MultiByteToWideChar(CP_ACP, 0, workPath.c_str(), -1, NULL, 0);
	LPWSTR filePath = new TCHAR[l];
	LPWSTR wkPath = new TCHAR[m];
	MultiByteToWideChar(CP_ACP, 0, appPath.c_str(), -1, filePath, l);
	MultiByteToWideChar(CP_ACP, 0, workPath.c_str(), -1, wkPath, m);
	ShellExecute(NULL, (LPCWSTR)L"open", filePath, NULL, wkPath, SW_SHOWNORMAL);
}

void Chimera::StartMelcorTermial()
{
	std::string appPath((QDir::currentPath() + "/bin/Melcor.exe").toLocal8Bit());
	std::string workPath(tworkdir.toLocal8Bit());
	int l = MultiByteToWideChar(CP_ACP, 0, appPath.c_str(), -1, NULL, 0);
	int m = MultiByteToWideChar(CP_ACP, 0, workPath.c_str(), -1, NULL, 0);
	LPWSTR filePath = new TCHAR[l];
	LPWSTR wkPath = new TCHAR[m];
	MultiByteToWideChar(CP_ACP, 0, appPath.c_str(), -1, filePath, l);
	MultiByteToWideChar(CP_ACP, 0, workPath.c_str(), -1, wkPath, m);
	ShellExecute(NULL, (LPCWSTR)L"open", filePath, NULL, wkPath, SW_SHOWNORMAL);
}

void Chimera::updateVarList(QString filepath, const QString &name, const QVector<int> &str)
{
	PTFile ptfile(filepath);
	QList<QVector<float>> tmp = ptfile.GetPlot(str);
	if (tmp.size() > 1) {
		varpool->insert(strippedName(filepath) + "_time", tmp.first());
		tmp.removeFirst();
		varpool->insert(name, tmp.first());
	}
	contentwidget->updateVars(varpool->GetNameList());
}

void Chimera::ClearVars()
{
	varpool->clear();
	contentwidget->updateVars(varpool->GetNameList());
}

ads::SectionContent::RefPtr Chimera::getCurrentSC()
{
	auto list = _sectionwidget->contents();
	int idx = _sectionwidget->currentIndex();
	return list.at(idx);
}
