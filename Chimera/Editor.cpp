#include "Editor.h"
#include "Application.h"
#include "Qsci/qsciscintilla.h"
#include "Qsci/qscilexerfortran.h"
#include "Qsci/qsciapis.h"
#include "QtAwesome/QtAwesome.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QLineEdit>
#include <QAction>
#include <QMenu>
#include <QDebug>
#include <QFontDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include <QFileDialog>
#include <QKeyEvent>

int Editor::count = 0;

Editor::Editor(QWidget * parent)
	:QWidget(parent)
{
	idclb = new QLabel;
	texteditor = new QsciScintilla;
	lineEdit = new QLineEdit;
	findwidget = new QWidget;
	filename= QStringLiteral("Untitled%1").arg(count);
	encoding = "UTF-8";
	texteditor->installEventFilter(this);
	initQsci(texteditor);
	textlexer = new QsciLexerFortran(texteditor);
	textlexer->setFont(QFont("Consolas"));
	textlexer->setFoldCompact(false);
	apis = new QsciAPIs(textlexer);
	QStringList keyword;
	auto loadkeywordlist = [&keyword](const QString &filename) {
		QFile file(filename);
		if (file.open(QFile::ReadOnly)) {
			while (!file.atEnd())
			{
				QByteArray line = file.readLine();
				if (!line.isEmpty()) {
					keyword << line.trimmed();
				}
			}
		}
	};
	loadkeywordlist(":/list/Resources/keywords.txt");
	for (auto i : keyword) {
		apis->add(i);
	}
	apis->prepare();
	setLexer(true);
	connect(texteditor, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(updateidclb(int, int)));

	constructUI();
	createMenu();
	++count;
}

Editor::~Editor()
{
}

QString Editor::FileName()const {
	return filename;
}

void Editor::setFile(const QString & name)
{
	if (name.isEmpty()) {
		return;
	}
	else
	{
		encoding = detectEncode(name);
		QFile file(name);
		if (!file.open(QFile::ReadOnly)) {
			qCritical() << QStringLiteral("%1:\n%2").arg(QDir::toNativeSeparators(name), file.errorString());
			return;
		}
		QTextStream in(&file);
		QTextCodec *codec_for_encoding = QTextCodec::codecForName(encoding.toUtf8());
		in.setCodec(codec_for_encoding);
		texteditor->setText(in.readAll());
		filename = name;
		texteditor->setModified(false);
		emit titlechanged(filename);
	}
}

bool Editor::save()
{
	QFileInfo info(filename);
	return info.exists() ? saveFile(filename):saveAs();
}

bool Editor::saveAs()
{
	QString Name = QFileDialog::getSaveFileName(this, tr("Save As"),filename, tr("text(*.txt);"));
	if (Name.isEmpty()) {
		return false;
	}
	return saveFile(Name);
}

bool Editor::saveFile(const QString & Name)
{
	QFile file(Name);
	if (!file.open(QFile::WriteOnly)) {
		QMessageBox::warning(this, tr("MIDK"),
			tr("Cannot write file %1:\n%2.")
			.arg(QDir::toNativeSeparators(Name), file.errorString()));
		return false;
	}
	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << texteditor->text();
	file.close();
	filename = Name;
	emit titlechanged(filename);
	texteditor->setModified(false);
	return true;
}

void Editor::constructUI()
{
	idclb->setAlignment(Qt::AlignRight);

	QVBoxLayout *mainlayout = new QVBoxLayout;
	QHBoxLayout *layout = new QHBoxLayout;
	QHBoxLayout *titlelayout = new QHBoxLayout;

	findwidget->setLayout(layout);
	layout->setContentsMargins(9, 4, 9, 4);
	mainlayout->setContentsMargins(0, 0, 0, 0);
	mainlayout->setSpacing(0);
	mainlayout->addWidget(idclb);
	mainlayout->addWidget(texteditor);
	mainlayout->addWidget(findwidget);
	setLayout(mainlayout);
	findwidget->hide();

	QToolButton* wraptoggleButton = new QToolButton(this);
	wraptoggleButton->setFont(myApp()->iconFont(14));
	wraptoggleButton->setText(QChar(fa::repeat));
	wraptoggleButton->setToolTip("Wrap Around");
	wraptoggleButton->setCheckable(true);
	layout->addWidget(wraptoggleButton, 0);

	QToolButton* retoggleButton = new QToolButton(this);
	retoggleButton->setFont(myApp()->iconFont(14));
	retoggleButton->setText(QChar(fa::asterisk));
	retoggleButton->setToolTip("Regular Expression");
	retoggleButton->setCheckable(true);
	layout->addWidget(retoggleButton, 0);

	QToolButton* cstoggleButton = new QToolButton(this);
	cstoggleButton->setFont(myApp()->iconFont(14));
	cstoggleButton->setText(QChar(fa::font));
	cstoggleButton->setToolTip("Case Sensitive");
	cstoggleButton->setCheckable(true);
	layout->addWidget(cstoggleButton, 0);

	QToolButton* wotoggleButton = new QToolButton(this);
	wotoggleButton->setFont(myApp()->iconFont(14));
	wotoggleButton->setText(QChar(fa::underline));
	wotoggleButton->setToolTip("Whole Word");
	wotoggleButton->setCheckable(true);
	layout->addWidget(wotoggleButton, 0);

	QAction *act = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::search)),"searchicon",this);
	//act->setEnabled(false);
	lineEdit->addAction(act, QLineEdit::LeadingPosition);
	lineEdit->setClearButtonEnabled(true);
	connect(lineEdit, &QLineEdit::returnPressed, [this, retoggleButton, wraptoggleButton, cstoggleButton, wotoggleButton] {
		this->FindText(retoggleButton->isChecked(), cstoggleButton->isChecked(), wotoggleButton->isChecked(), wraptoggleButton->isChecked(), true);
	});
	layout->addWidget(lineEdit, 1);

	QToolButton* butpre = new QToolButton(this);
	butpre->setFont(myApp()->iconFont(14));
	butpre->setText(QChar(fa::caretleft));
	butpre->setToolTip(QString(tr("Find Previous (%1)")).arg(butpre->shortcut().toString()));
	connect(butpre, &QToolButton::pressed, [this,retoggleButton,wraptoggleButton,cstoggleButton,wotoggleButton] {
		this->FindText(retoggleButton->isChecked(),cstoggleButton->isChecked(),wotoggleButton->isChecked(),wraptoggleButton->isChecked(),false);
	});
	layout->addWidget(butpre, 0);

	QToolButton* butnet = new QToolButton(this);
	butnet->setFont(myApp()->iconFont(14));
	butnet->setText(QChar(fa::caretright));
	butnet->setToolTip(QString(tr("Find Next (%1)")).arg(butnet->shortcut().toString()));
	connect(butnet, &QToolButton::pressed, [this, retoggleButton, wraptoggleButton, cstoggleButton, wotoggleButton] {
		this->FindText(retoggleButton->isChecked(), cstoggleButton->isChecked(), wotoggleButton->isChecked(), wraptoggleButton->isChecked(), true);
	});
	layout->addWidget(butnet, 0);

	QToolButton *but = new QToolButton(this);
	but->setFont(myApp()->iconFont(14));
	but->setText(QChar(fa::remove));
	but->setShortcut(Qt::Key_Escape);
	connect(but, &QToolButton::pressed,this,&Editor::hideFind);
	layout->addWidget(but, 0);
}

void Editor::createMenu()
{
	menu = new QMenu(this);

	QAction *undoAct = new QAction("Undo", this);
	undoAct->setIcon(QIcon(myApp()->qtAwesome()->icon(fa::undo)));
	connect(undoAct, SIGNAL(triggered()),texteditor,SLOT(undo()));
	menu->addAction(undoAct);

	QAction *redoAct = new QAction("Redo", this);
	redoAct->setIcon(QIcon(myApp()->qtAwesome()->icon(fa::repeat)));
	connect(redoAct, SIGNAL(triggered()), texteditor, SLOT(redo()));
	menu->addAction(redoAct);

	menu->addSeparator();

#ifndef QT_NO_CLIPBOARD
	QAction *cutAct = new QAction("Cut", this);
	cutAct->setIcon(QIcon(myApp()->qtAwesome()->icon(fa::cut)));
	connect(cutAct, SIGNAL(triggered()), texteditor, SLOT(cut()));
	menu->addAction(cutAct);

	QAction *copyAct = new QAction("Copy", this);
	copyAct->setIcon(QIcon(myApp()->qtAwesome()->icon(fa::copy)));
	connect(copyAct, SIGNAL(triggered()), texteditor, SLOT(copy()));
	menu->addAction(copyAct);

	QAction *pasteAct = new QAction("Paste", this);
	pasteAct->setIcon(QIcon(myApp()->qtAwesome()->icon(fa::paste)));
	connect(pasteAct, SIGNAL(triggered()), texteditor, SLOT(paste()));
	menu->addAction(pasteAct);
#endif // !QT_NO_CLIPBOARD

#ifndef QT_NO_CLIPBOARD
	cutAct->setEnabled(false);
	copyAct->setEnabled(false);
	connect(texteditor, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
	connect(texteditor, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
#endif // !QT_NO_CLIPBOARD
	menu->addSeparator();

	QAction *foldAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::code)),"Fold/Unfold", this);
	connect(foldAct, &QAction::triggered, texteditor, &QsciScintilla::foldAll);
	connect(this, &Editor::lexerchanged, [foldAct](bool t) {foldAct->setEnabled(t); });
	menu->addAction(foldAct);

	QAction *selAct = new QAction("Select All", this);
	connect(selAct, SIGNAL(triggered()), texteditor, SLOT(selectAll()));
	menu->addAction(selAct);

	QMenu *convertMenu = new QMenu("Text Sorting", menu);
	QAction *toUpperAct = new QAction("to Upper", this);
	connect(toUpperAct, &QAction::triggered, [this] {this->SortText(1); });
	convertMenu->addAction(toUpperAct);
	QAction *toLowwerAct = new QAction("to Lower", this);
	connect(toLowwerAct, &QAction::triggered, [this] {this->SortText(2); });
	convertMenu->addAction(toLowwerAct);
	QAction *sortaAct = new QAction("Ascending", this);
	sortaAct->setIcon(QIcon(myApp()->qtAwesome()->icon(fa::sortalphaasc)));
	connect(sortaAct, &QAction::triggered, [this] {this->SortText(3); });
	convertMenu->addAction(sortaAct);
	QAction *sortdAct = new QAction("Descending", this);
	sortdAct->setIcon(QIcon(myApp()->qtAwesome()->icon(fa::sortalphadesc)));
	connect(sortdAct, &QAction::triggered, [this] {this->SortText(4); });
	convertMenu->addAction(sortdAct);

	menu->addMenu(convertMenu);

	QAction *findAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::search)), "Find", this);
	connect(findAct, &QAction::triggered, this, &Editor::Find);
	menu->addAction(findAct);

	menu->addSeparator();

	QAction *SaveAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::save)), "Save", this);
	connect(SaveAct, &QAction::triggered, this, &Editor::save);
	menu->addAction(SaveAct);

	QAction *SaveAsAct = new QAction(QIcon(), "Save as", this);
	connect(SaveAsAct, &QAction::triggered, this, &Editor::saveAs);
	menu->addAction(SaveAsAct);

	menu->addSeparator();

	QAction *MarcoAct = new QAction(QIcon(), "Process Marcos", this);
	connect(MarcoAct, &QAction::triggered, this, &Editor::processMarcos);
	menu->addAction(MarcoAct);

	QAction *compileAct = new QAction(QIcon(), "Compile Current", this);
	connect(compileAct, &QAction::triggered, this, &Editor::CompileCurFile);
	menu->addAction(compileAct);

	QAction *RunAct = new QAction(QIcon(), "Run Current", this);
	connect(RunAct, &QAction::triggered, [this] {emit RequireRunCur(FileName(), QStringLiteral("")); });
	menu->addAction(RunAct);

	menu->addSeparator();

	QMenu *settingmenu = new QMenu("Editor Settings",menu);
	QAction *LexerAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::language)),"Text Lexer On",this);
	LexerAct->setCheckable(true);
	LexerAct->setChecked(true);
	connect(LexerAct, &QAction::toggled, this, &Editor::setLexer);
	connect(LexerAct, &QAction::toggled, [LexerAct] (bool t){
		LexerAct->setText(t?"Text Lexer On" : "Text Lexer Off");
	});
	settingmenu->addAction(LexerAct);
	QAction *fontAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::font)), "Text Font", this);
	connect(fontAct, &QAction::triggered, this, &Editor::setFont);
	connect(this, &Editor::lexerchanged, [fontAct](bool t) {
		fontAct->setEnabled(t);
	});
	settingmenu->addAction(fontAct);
	QAction *visibleAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::eyeslash)), "Non-print Invisiable", this);
	visibleAct->setCheckable(true);
	visibleAct->setChecked(false);
	connect(visibleAct, &QAction::toggled,this,&Editor::setVisiablity);
	connect(visibleAct, &QAction::toggled, [visibleAct](bool t) {
		visibleAct->setIcon(t ? QIcon(myApp()->qtAwesome()->icon(fa::eye)):QIcon(myApp()->qtAwesome()->icon(fa::eyeslash)));
		visibleAct->setText(t ? "Non-print Visiable" : "Non-print Invisiable");
	});
	settingmenu->addAction(visibleAct);
	menu->addMenu(settingmenu);
}

void Editor::setLexer(bool t)
{
	if (t)
	{
		SpecifiedQsci(texteditor, textlexer);
	}
	else
	{
		texteditor->setLexer(nullptr);
		texteditor->setMarginsBackgroundColor(Qt::white);
		texteditor->setFolding(QsciScintilla::NoFoldStyle);
	}
	setEditMargin();
	emit lexerchanged(t);
}

void Editor::setFont()
{
	if (texteditor->lexer() != nullptr)
	{
		QFont font = QFontDialog::getFont(0, texteditor->lexer()->font(0), this);
		texteditor->lexer()->setFont(font);
		texteditor->setMarginsFont(font);
	}
	else
	{
		qWarning() << QStringLiteral("setFont() is not available in plain text mode.");
	}
}

void Editor::initQsci(QsciScintilla * textedit)
{
	textedit->setFrameShape(QFrame::NoFrame);
	textedit->setContextMenuPolicy(Qt::CustomContextMenu);
	textedit->setEdgeMode(QsciScintilla::EdgeLine);
	connect(textedit, &QsciScintilla::customContextMenuRequested, this, &Editor::contextMenuSlot);
	textedit->setMarginType(0, QsciScintilla::NumberMargin);
	textedit->setMarginLineNumbers(0, true);
	textedit->setMarginSensitivity(0, true);

	textedit->setMarginType(1, QsciScintilla::SymbolMargin);
	textedit->setMarginLineNumbers(1, false);
	textedit->setMarginWidth(1, 4);
	textedit->setMarginSensitivity(1, true);
	textedit->setMarginMarkerMask(1, 0x02);
	textedit->markerDefine(QsciScintilla::FullRectangle, 1);
	textedit->setMarkerBackgroundColor(Qt::darkCyan, 1);

	textedit->setCaretLineVisible(true);
	textedit->setCaretLineBackgroundColor(QColor(Qt::yellow).lighter(160));

	textedit->SendScintilla(QsciScintilla::SCI_SETMULTIPLESELECTION, 1);
	textedit->SendScintilla(QsciScintilla::SCI_SETADDITIONALSELECTIONTYPING, 1);
	textedit->SendScintilla(QsciScintilla::SCI_SETMULTIPASTE, QsciScintillaBase::SC_MULTIAUTOC_EACH);
	textedit->SendScintilla(QsciScintilla::SCI_SETVIRTUALSPACEOPTIONS, 1);

	textedit->setUtf8(true);
	textedit->setAutoIndent(true);
	textedit->setTabWidth(4);
	textedit->setIndentationsUseTabs(false);
	textedit->setIndentationGuides(true);
	textedit->setTabIndents(true);
	textedit->setEolMode(QsciScintilla::EolUnix);

	connect(textedit, SIGNAL(linesChanged()), this, SLOT(setEditMargin()));

	QPalette palette = textedit->palette();
	palette.setColor(QPalette::Inactive, QPalette::HighlightedText,
		palette.color(QPalette::Active, QPalette::HighlightedText));
	palette.setColor(QPalette::Inactive, QPalette::Highlight,
		palette.color(QPalette::Active, QPalette::Highlight));
	textedit->setPalette(palette);
}

void Editor::SpecifiedQsci(QsciScintilla * textedit, QsciLexerFortran * lexer)
{
	textedit->setMarginsFont(QFont("Consolas"));
	textedit->setMarginsBackgroundColor(Qt::white);
	textedit->setFolding(QsciScintilla::BoxedTreeFoldStyle);
	textedit->setFoldMarginColors(Qt::white, Qt::white);
	//textedit->setMarginWidth(2, 16);
	textedit->markerDefine(QsciScintilla::DownTriangle, QsciScintilla::SC_MARKNUM_FOLDEROPEN);
	textedit->markerDefine(QsciScintilla::RightTriangle, QsciScintilla::SC_MARKNUM_FOLDER);
	textedit->markerDefine(QsciScintilla::DownTriangle, QsciScintilla::SC_MARKNUM_FOLDEROPENMID);
	textedit->markerDefine(QsciScintilla::RightTriangle, QsciScintilla::SC_MARKNUM_FOLDEREND);
	textedit->markerDefine(QsciScintilla::ThreeDots, QsciScintilla::QsciScintilla::SC_MARKNUM_FOLDERSUB);
	textedit->markerDefine(QsciScintilla::ThreeDots, QsciScintilla::QsciScintilla::SC_MARKNUM_FOLDERMIDTAIL);
	textedit->markerDefine(QsciScintilla::ThreeDots, QsciScintilla::QsciScintilla::SC_MARKNUM_FOLDERTAIL);

	//textedit->setEdgeMode(QsciScintilla::EdgeLine)
	textedit->setLexer(lexer);

	textedit->setAutoCompletionSource(QsciScintilla::AcsAll);
	textedit->setAutoCompletionThreshold(2);
}

int Editor::caculateoffset(int line, int index)
{
	int offset = 0;
	for (int i = 0; i < line; ++i) {
		offset = offset + texteditor->lineLength(i);
	}
	return offset + index;
}

void Editor::setEditMargin()
{
	int num = texteditor->lines();
	QString placeholder = "x";
	while (num != 0)
	{
		num = num / 10;
		placeholder = placeholder + "x";
	}
	texteditor->setMarginWidth(0, placeholder);
}

void Editor::contextMenuSlot(QPoint p)
{
	menu->exec(mapToGlobal(p));
}

void Editor::Find()
{
	lineEdit->setText(texteditor->selectedText());
	findwidget->setVisible(true);
	texteditor->setCursorPosition(0, 0);
}

void Editor::hideFind()
{
	findwidget->hide();
}

void Editor::setVisiablity(bool t)
{
	texteditor->setEolVisibility(t);
	texteditor->setWhitespaceVisibility(t?QsciScintilla::WsVisible:QsciScintilla::WsInvisible);
}

void Editor::processMarcos()
{
	if (save()) {
		texteditor->beginUndoAction();
		texteditor->clear();
		texteditor->insert(trim(filename));
		texteditor->endUndoAction();
	}
}

void Editor::CompileCurFile()
{
	if (!maybeSave()) {
		qWarning() << "file is unsaved.";
		return;
	}
	InputDeck::check(filename);
}

void Editor::FindText(bool re, bool cs, bool wo, bool wrap, bool forward)
{
	if (!forward) {
		int a, b;
		int length = lineEdit->text().length();
		texteditor->getCursorPosition(&a, &b);
		while (b < length)
		{
			b = b + texteditor->lineLength(a);
			a = a - 1;
		}
		texteditor->setCursorPosition(a, b - length);
	}
	bool fd = texteditor->findFirst(lineEdit->text(), re, cs, wo, wrap, forward);
	if (!fd) {
		QMessageBox::information(this,QStringLiteral("Find-%1").arg(lineEdit->text()),"End of search.",QMessageBox::Ok);
	}
}

bool Editor::isModified()
{
	return texteditor->isModified();
}

bool Editor::maybeSave()
{
	if (!texteditor->isModified())
	{
		return true;
	}
	const QMessageBox::StandardButton ret
		= QMessageBox::warning(this, filename,
			tr("The document has been modified.\n"
				"Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard
			| QMessageBox::Cancel);
	switch (ret) {
	case QMessageBox::Save:
		return save();
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}
	return true;
}

QString Editor::strippedName(const QString & fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

bool Editor::eventFilter(QObject * target, QEvent * e)
{
	if (target == texteditor) {
		if (e->type() == QEvent::KeyPress) {
			QKeyEvent *event = static_cast<QKeyEvent *>(e);
			if (event->modifiers() == Qt::ControlModifier&&event->key() == Qt::Key_F) {
				Find();
				return true;
			}
			else if(event->modifiers() == Qt::ControlModifier&&event->key() == Qt::Key_S)
			{
				save();
				return true;
			}
		}
	}
	return QWidget::eventFilter(target,e);
}

void Editor::SortText(int mode)
{
	QString tmp = texteditor->selectedText();
	QStringList list = tmp.split("\n");
	switch (mode)
	{
	case 1://to upper
		texteditor->replaceSelectedText(tmp.toUpper());
		break;
	case 2://to lowwer
		texteditor->replaceSelectedText(tmp.toLower());
		break;
	case 3://sort ascending
		list.sort(Qt::CaseInsensitive);
		texteditor->replaceSelectedText(list.join("\n"));
		break;
	case 4://sort descending
		list.sort(Qt::CaseInsensitive);
		std::reverse(list.begin(), list.end());
		texteditor->replaceSelectedText(list.join("\n"));
		break;
	default:
		qWarning() << QStringLiteral("undefined mode.");
		break;
	}
}

void Editor::updateidclb(int line,int index) {
	idclb->setText(QStringLiteral("Line:%1(%2) | Column:%3(%4) | Offset:%5(%6) | %7 ")
		.arg(line+1).arg(texteditor->lines())
		.arg(index).arg(texteditor->lineLength(line))
		.arg(caculateoffset(line,index)).arg(texteditor->length())
		.arg(encoding));
}

QString detectEncode(const QString & filename)
{
	QFile input_file(filename);
	QByteArray chunk;
	if (input_file.open(QFile::ReadOnly)) {
		chunk = input_file.readAll();
		input_file.close();
	}
	int len = chunk.length();
	char* buf = chunk.data();
	uchardet_t ud = uchardet_new();

	if (uchardet_handle_data(ud, buf, len) != 0) {
		qWarning() << QStringLiteral("Detecting encoding failed.");
	};
	uchardet_data_end(ud);
	QByteArray encoding_name = QByteArray(uchardet_get_charset(ud));
	uchardet_delete(ud);

	QString str(encoding_name);
	if (str == "ASCII") {
		encoding_name = "UTF-8";
	}
	return str;
}
