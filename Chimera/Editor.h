#pragma once
#include <QWidget>
#include "uchardet/uchardet.h"
#include "InputDeck.h"

QString detectEncode(const QString &filename);

class QsciScintilla;
class QsciLexerFortran;
class QsciAPIs;
class QLineEdit;
class QAction;
class QLabel;
class QMenu;

class Editor :
	public QWidget
{
	Q_OBJECT
public:
	Editor(QWidget *parent = nullptr);
	~Editor();
	QString FileName() const;
	void setFile(const QString &filename);
	bool save();
	bool saveAs();
	bool saveFile(const QString &filename);
	void SortText(int mode);
	void FindText(bool re, bool cs, bool wo, bool wrap, bool forward);
	static int count;
	bool isModified();
	bool maybeSave();
	static QString strippedName(const QString &fullFileName);
signals:
	void lexerchanged(bool t);
	void titlechanged(QString title);
	void RequireRunCur(const QString &filename, const QString &path);
protected:
	bool eventFilter (QObject *target,QEvent *e) override;
private:
	QsciScintilla *texteditor;
	QsciLexerFortran *textlexer;
	QsciAPIs *apis;
	QLineEdit *lineEdit;
	QMenu *menu;
	QWidget *findwidget;
	QLabel *idclb;
	QString filename;
	QString encoding;
private:
	void constructUI();
	void createMenu();
	void setLexer(bool t);
	void setFont();
	void initQsci(QsciScintilla * textedit);
	void SpecifiedQsci(QsciScintilla *edit,QsciLexerFortran *lexer);
	int caculateoffset(int line, int index);
private slots:
	void updateidclb(int, int);
	void setEditMargin();
	void contextMenuSlot(QPoint p);
	void Find();
	void hideFind();
	void setVisiablity(bool t);
	void processMarcos();
	void CompileCurFile();
};