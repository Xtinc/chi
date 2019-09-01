#pragma once
#include <QtWidgets/QMainWindow>
#include "ads/SectionContent.h"
#include "Editor.h"
#include "FindWidget.h"
#include "PTFWidget.h"
#include "GraphWidget.h"
#include "Application.h"
#include "IconTitleWidget.h"
#include "qtdiff/diff/zpathdiffmodel.h"
#include "qtdiff/widget/folder/zfolderwidget.h"

class QPlainTextEdit;
class QMessageLogContext;
class QMenu;
class ContentWidget;
class VarPool;

namespace ads {
	class ContainerWidget;
	class SectionWidget;
}

class Chimera : public QMainWindow
{
	Q_OBJECT

public:
	Chimera(QWidget *parent = Q_NULLPTR);
	~Chimera();
	QPlainTextEdit *console() const;
	ads::SectionContent::RefPtr getCurrentSC();
	VarPool *varpool;

public:
	enum WindowType
	{
		File,
		Plot,
		Find,
		Graph,
		Diff
	};
	enum { MaxRecentFiles = 8 };

protected:
	void closeEvent(QCloseEvent *e) override;
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent *event) override;

private:
	void ConstructConsole();
	void ConstructMenu();
	void ConstructContent();
	//ads::SectionContent::RefPtr ConstructWin(ads::ContainerWidget *_container,WindowType type);
	void updateWindowMenu();
	void newTextFile();
	void newPlotFile();
	void Open();
	void OpenFile(const QString &str);
	void closeAll();
	void closeHidden();
	static bool hasRecentFiles();
	void prependToRecentFiles(const QString &fileName);
	void removeInvalidFiles(const QString &fileName);
	void setRecentFilesVisible(bool visible);
	void updateRecentFileActions();
	void openRecentFile();
	void showFindDig();
	void showDiffDig();
	void fileCompare(const ZPathDiffModel &pathDiffModel);
	void CreatePTFPlot(QString filepath, const QStringList &name, const QVector<int> &str);
	void setTworkdir();
	void StartMelgenTerminal();
	void StartMelcorTermial();
	void updateVarList(QString filepath, const QString &name,const QVector<int> &str);
	void ClearVars();
private:
	ads::ContainerWidget *_container;//main window
	ads::SectionWidget *_sectionwidget;//layout container
	QPlainTextEdit *_console;//log console
	ContentWidget *contentwidget;//welcome window
	QMenu *viewMenu;
	QMenu *recentMenu;
	QAction *recentFileActs[MaxRecentFiles];
	QAction *recentFileSeparator;
	QAction *recentFileSubMenuAct;
	QString tworkdir;
private:
	template<typename T>
	bool isTpye(WindowType type);
	template<typename T>
	ads::SectionContent::RefPtr ConstructWin(T t, ads::ContainerWidget *_container);
};

template<typename T>
inline bool Chimera::isTpye(WindowType type)
{
	return false;
}
template<>
inline bool Chimera::isTpye<Editor*>(WindowType type) {
	if (type == File) {
		return true;
	}
	return false;
}
template<>
inline bool Chimera::isTpye<FindWidget*>(WindowType type) {
	if (type == Find) {
		return true;
	}
	return false;
}
template<>
inline bool Chimera::isTpye<PTFWidget*>(WindowType type) {
	if (type == Plot) {
		return true;
	}
	return false;
}
template<>
inline bool Chimera::isTpye<GraphWidget*>(WindowType type) {
	if (type == Graph) {
		return true;
	}
	return false;
}
template<>
inline bool Chimera::isTpye<ZFolderWidget*>(WindowType type) {
	if (type == Diff) {
		return true;
	}
	return false;
}

template<typename T>
inline ads::SectionContent::RefPtr Chimera::ConstructWin(T t, ads::ContainerWidget * _container)
{
	ads::SectionContent::RefPtr sc;
	if (isTpye<T>(File)) {
		Editor* edit = dynamic_cast<Editor*>(t);
		IconTitleWidget *title = new IconTitleWidget(QIcon(myApp()->qtAwesome()->icon(fa::code)), "");
		sc = ads::SectionContent::newSectionContent(QStringLiteral("File%1").arg(edit->count), _container, title, edit);
		auto fun = [sc, title, edit](QString str) {
			title->setTitle(edit->strippedName(str));
			title->setToolTip(str);
			sc->setTitle(str);
		};
		connect(edit, &Editor::titlechanged, fun);
		fun(edit->FileName());
	}
	else if (isTpye<T>(Find)) {
		FindWidget *findwidget = dynamic_cast<FindWidget*>(t);
		IconTitleWidget *title = new IconTitleWidget(QIcon(myApp()->qtAwesome()->icon(fa::search)), "");
		sc = ads::SectionContent::newSectionContent(QStringLiteral("Find%1").arg(findwidget->count), _container, title, findwidget);
		QString task = "Find-"+findwidget->taskname();
		title->setTitle(task);
		title->setToolTip(task);
		sc->setTitle(task);
	}
	else if (isTpye<T>(Plot)) {
		PTFWidget *plotwidget = dynamic_cast<PTFWidget*>(t);
		IconTitleWidget *title = new IconTitleWidget(QIcon(myApp()->qtAwesome()->icon(fa::filepictureo)), "");
		sc = ads::SectionContent::newSectionContent(QStringLiteral("Plot%1").arg(plotwidget->count), _container, title,plotwidget);
		QString task = "PTF-" + plotwidget->taskname();
		title->setTitle(task);
		title->setToolTip(task);
		sc->setTitle(task);
	}
	else if(isTpye<T>(Graph)){
		GraphWidget *graphwidget = dynamic_cast<GraphWidget*>(t);
		IconTitleWidget *title = new IconTitleWidget(QIcon(myApp()->qtAwesome()->icon(fa::linechart)), "");
		sc = ads::SectionContent::newSectionContent(QStringLiteral("Graph%1").arg(graphwidget->count), _container, title, graphwidget);
		QString task = "Graph-" + QString::number(graphwidget->count);
		title->setTitle(task);
		title->setToolTip(task);
		sc->setTitle(task);
	}
	else if (isTpye<T>(Diff)) {
		ZFolderWidget *folderWidget = dynamic_cast<ZFolderWidget*>(t);
		IconTitleWidget *title = new IconTitleWidget(QIcon(myApp()->qtAwesome()->icon(fa::copy)), "");
		sc = ads::SectionContent::newSectionContent(QStringLiteral("Diff%1").arg(folderWidget->count), _container, title, folderWidget);
		QString task = "Comparison-" + QString::number(folderWidget->count);
		title->setTitle(task);
		title->setToolTip(task);
		sc->setTitle(task);
	}
	return sc;
}

static inline QString recentFilesKey() { return QStringLiteral("recentFileList"); }

static inline QString fileKey() { return QStringLiteral("file"); }

static QStringList readRecentFiles(QSettings &settings)
{
	QStringList result;
	const int count = settings.beginReadArray(recentFilesKey());
	for (int i = 0; i < count; ++i) {
		settings.setArrayIndex(i);
		result.append(settings.value(fileKey()).toString());
	}
	settings.endArray();
	return result;
}

static void writeRecentFiles(const QStringList &files, QSettings &settings)
{
	const int count = files.size();
	settings.beginWriteArray(recentFilesKey());
	for (int i = 0; i < count; ++i) {
		settings.setArrayIndex(i);
		settings.setValue(fileKey(), files.at(i));
	}
	settings.endArray();
}