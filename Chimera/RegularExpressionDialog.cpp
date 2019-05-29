#include "RegularExpressionDialog.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QSpinBox>
#include <QPlainTextEdit>
#include <QTreeWidget>
#include <QDir>
#include <QFileDialog>
#include <QPushButton>
#include <QAction>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>

#include"QtAwesome/QtAwesome.h"
#include "application.h"

Q_DECLARE_METATYPE(QRegularExpression::MatchType)

static QString rawStringLiteral(QString pattern)
{
	pattern.prepend(QLatin1String("R\"RX("));
	pattern.append(QLatin1String(")RX\""));
	return pattern;
}

static QString patternToCode(QString pattern)
{
	pattern.replace(QLatin1String("\\"), QLatin1String("\\\\"));
	pattern.replace(QLatin1String("\""), QLatin1String("\\\""));
	pattern.prepend(QLatin1Char('"'));
	pattern.append(QLatin1Char('"'));
	return pattern;
}

static QString codeToPattern(QString code)
{
	for (int i = 0; i < code.size(); ++i) {
		if (code.at(i) == QLatin1Char('\\'))
			code.remove(i, 1);
	}
	if (code.startsWith(QLatin1Char('"')) && code.endsWith(QLatin1Char('"'))) {
		code.chop(1);
		code.remove(0, 1);
	}
	return code;
}

PatternLineEdit::PatternLineEdit(QWidget *parent)
	:QLineEdit(parent),
	escapeSelectionAction(new QAction(tr("Escape Selection"), this)),
	copyToCodeAction(new QAction(tr("Copy to Code"), this)),
	pasteFromCodeAction(new QAction(tr("Paste from Code"), this)) {
	setClearButtonEnabled(true);
	connect(escapeSelectionAction, &QAction::triggered, this, &PatternLineEdit::escapeSelection);
	connect(copyToCodeAction, &QAction::triggered, this, &PatternLineEdit::copyToCode);
	connect(pasteFromCodeAction, &QAction::triggered, this, &PatternLineEdit::pasteFromCode);
#if !QT_CONFIG(clipboard)
	copyToCodeAction->setEnabled(false);
	pasteFromCodeAction->setEnabled(false);
#endif
}

void PatternLineEdit::escapeSelection() {
	const QString selection = selectedText();
	const QString escapedSelection = QRegularExpression::escape(selection);
	if (escapedSelection != selection) {
		QString t = text();
		t.replace(selectionStart(), selection.size(), escapedSelection);
		setText(t);
	}
}

void PatternLineEdit::copyToCode()
{
#if QT_CONFIG(clipboard)
	QGuiApplication::clipboard()->setText(patternToCode(text()));
#endif
}

void PatternLineEdit::pasteFromCode()
{
#if QT_CONFIG(clipboard)
	setText(codeToPattern(QGuiApplication::clipboard()->text()));
#endif
}

void PatternLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu *menu = createStandardContextMenu();
	menu->setAttribute(Qt::WA_DeleteOnClose);
	menu->addSeparator();
	escapeSelectionAction->setEnabled(hasSelectedText());
	menu->addAction(escapeSelectionAction);
	menu->addSeparator();
	menu->addAction(copyToCodeAction);
	menu->addAction(pasteFromCodeAction);
	menu->popup(event->globalPos());
}

DisplayLineEdit::DisplayLineEdit(QWidget *parent) : QLineEdit(parent)
{
	setReadOnly(true);
	QPalette disabledPalette = palette();
	disabledPalette.setBrush(QPalette::Base, disabledPalette.brush(QPalette::Disabled, QPalette::Base));
	setPalette(disabledPalette);

#if QT_CONFIG(clipboard)
	QAction *copyAction = new QAction(this);
	copyAction->setText(RegularExpressionDialog::tr("Copy to clipboard"));
	copyAction->setIcon(QIcon(QStringLiteral(":/images/copy.png")));
	connect(copyAction, &QAction::triggered, this,
		[this]() { QGuiApplication::clipboard()->setText(text()); });
	addAction(copyAction, QLineEdit::TrailingPosition);
#endif
}

RegularExpressionDialog::RegularExpressionDialog(QWidget *parent)
	:QDialog(parent) {
	setupUi();
	setWindowTitle(tr("RegularExpression Test"));

	connect(patternLineEdit, &QLineEdit::textChanged, this, &RegularExpressionDialog::refresh);
	connect(subjectTextEdit, &QPlainTextEdit::textChanged, this, &RegularExpressionDialog::refresh);

	connect(caseInsensitiveOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);
	connect(dotMatchesEverythingOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);
	connect(multilineOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);
	connect(extendedPatternSyntaxOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);
	connect(invertedGreedinessOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);
	connect(dontCaptureOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);
	connect(useUnicodePropertiesOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);
	connect(optimizeOnFirstUsageOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);
	connect(dontAutomaticallyOptimizeOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);

	connect(offsetSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
		this, &RegularExpressionDialog::refresh);

	connect(matchTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, &RegularExpressionDialog::refresh);

	connect(anchoredMatchOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);
	connect(dontCheckSubjectStringMatchOptionCheckBox, &QCheckBox::toggled, this, &RegularExpressionDialog::refresh);

	patternLineEdit->setText(tr("(\\+?\\d+)-(?<prefix>\\d+)-(?<number>\\w+)"));
	subjectTextEdit->setPlainText(tr("Test:test number match +43-152-0123456, this dialog is 001-41-255512 used"
		" for testing and converting Regular Expression."));

	refresh();
}

QString RegularExpressionDialog::text() const
{
	return patternLineEdit->text();
}

QString RegularExpressionDialog::dir() const
{
	return directoryLineEdit->text();
}

QString RegularExpressionDialog::filter() const
{
	return filterLineEdit->text();
}

bool RegularExpressionDialog::isREG() const
{
	return isreg->isChecked();
}

void RegularExpressionDialog::setResultUiEnabled(bool enabled)
{
	matchDetailsTreeWidget->setEnabled(enabled);
	namedGroupsTreeWidget->setEnabled(enabled);
}

static void setTextColor(QWidget *widget, const QColor &color)
{
	QPalette palette = widget->palette();
	palette.setColor(QPalette::Text, color);
	widget->setPalette(palette);
}

void RegularExpressionDialog::refresh()
{
	setUpdatesEnabled(false);

	const QString pattern = patternLineEdit->text();
	const QString text = subjectTextEdit->toPlainText();

	offsetSpinBox->setMaximum(qMax(0, text.length() - 1));

	escapedPatternLineEdit->setText(patternToCode(pattern));
	rawStringLiteralLineEdit->setText(rawStringLiteral(pattern));

	setTextColor(patternLineEdit, subjectTextEdit->palette().color(QPalette::Text));
	matchDetailsTreeWidget->clear();
	namedGroupsTreeWidget->clear();
	regexpStatusLabel->setText(QString());

	if (pattern.isEmpty()) {
		setResultUiEnabled(false);
		setUpdatesEnabled(true);
		return;
	}

	QRegularExpression rx(pattern);
	if (!rx.isValid()) {
		setTextColor(patternLineEdit, Qt::red);
		regexpStatusLabel->setText(tr("Invalid: syntax error at position %1 (%2)")
			.arg(rx.patternErrorOffset())
			.arg(rx.errorString()));
		setResultUiEnabled(false);
		setUpdatesEnabled(true);
		return;
	}

	setResultUiEnabled(true);

	QRegularExpression::MatchType matchType = matchTypeComboBox->currentData().value<QRegularExpression::MatchType>();
	QRegularExpression::PatternOptions patternOptions = QRegularExpression::NoPatternOption;
	QRegularExpression::MatchOptions matchOptions = QRegularExpression::NoMatchOption;

	if (anchoredMatchOptionCheckBox->isChecked())
		matchOptions |= QRegularExpression::AnchoredMatchOption;
	if (dontCheckSubjectStringMatchOptionCheckBox->isChecked())
		matchOptions |= QRegularExpression::DontCheckSubjectStringMatchOption;

	if (caseInsensitiveOptionCheckBox->isChecked())
		patternOptions |= QRegularExpression::CaseInsensitiveOption;
	if (dotMatchesEverythingOptionCheckBox->isChecked())
		patternOptions |= QRegularExpression::DotMatchesEverythingOption;
	if (multilineOptionCheckBox->isChecked())
		patternOptions |= QRegularExpression::MultilineOption;
	if (extendedPatternSyntaxOptionCheckBox->isChecked())
		patternOptions |= QRegularExpression::ExtendedPatternSyntaxOption;
	if (invertedGreedinessOptionCheckBox->isChecked())
		patternOptions |= QRegularExpression::InvertedGreedinessOption;
	if (dontCaptureOptionCheckBox->isChecked())
		patternOptions |= QRegularExpression::DontCaptureOption;
	if (useUnicodePropertiesOptionCheckBox->isChecked())
		patternOptions |= QRegularExpression::UseUnicodePropertiesOption;
	if (optimizeOnFirstUsageOptionCheckBox->isChecked())
		patternOptions |= QRegularExpression::OptimizeOnFirstUsageOption;
	if (dontAutomaticallyOptimizeOptionCheckBox->isChecked())
		patternOptions |= QRegularExpression::DontAutomaticallyOptimizeOption;

	rx.setPatternOptions(patternOptions);

	const int capturingGroupsCount = rx.captureCount() + 1;

	QRegularExpressionMatchIterator iterator = rx.globalMatch(text, offsetSpinBox->value(), matchType, matchOptions);
	int i = 0;

	while (iterator.hasNext()) {
		QRegularExpressionMatch match = iterator.next();

		QTreeWidgetItem *matchDetailTopItem = new QTreeWidgetItem(matchDetailsTreeWidget);
		matchDetailTopItem->setText(0, QString::number(i));

		for (int captureGroupIndex = 0; captureGroupIndex < capturingGroupsCount; ++captureGroupIndex) {
			QTreeWidgetItem *matchDetailItem = new QTreeWidgetItem(matchDetailTopItem);
			matchDetailItem->setText(1, QString::number(captureGroupIndex));
			matchDetailItem->setText(2, match.captured(captureGroupIndex));
		}

		++i;
	}

	matchDetailsTreeWidget->expandAll();

	regexpStatusLabel->setText(tr("Valid"));

	const QStringList namedCaptureGroups = rx.namedCaptureGroups();
	for (int i = 0; i < namedCaptureGroups.size(); ++i) {
		const QString currentNamedCaptureGroup = namedCaptureGroups.at(i);

		QTreeWidgetItem *namedGroupItem = new QTreeWidgetItem(namedGroupsTreeWidget);
		namedGroupItem->setText(0, QString::number(i));
		namedGroupItem->setText(1, currentNamedCaptureGroup.isNull() ? tr("<no name>") : currentNamedCaptureGroup);
	}

	setUpdatesEnabled(true);
}

void RegularExpressionDialog::setupUi()
{
	QWidget *leftHalfContainer = setUpLeftUi();

	QFrame *verticalSeparator = new QFrame;
	verticalSeparator->setFrameStyle(QFrame::VLine | QFrame::Sunken);
	QFrame *horizontalSeparator = new QFrame;
	horizontalSeparator->setFrameStyle(QFrame::HLine | QFrame::Sunken);

	QWidget *rightHalfContainer = setUpRightUi();

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(leftHalfContainer);
	mainLayout->addWidget(verticalSeparator);
	mainLayout->addWidget(rightHalfContainer);

	QHBoxLayout *seclayout = new QHBoxLayout;
	QPushButton *okbtn = new QPushButton("Find");
	connect(okbtn, &QPushButton::pressed, this, &RegularExpressionDialog::accept);
	QPushButton *calbtn = new QPushButton("Cancel");
	connect(calbtn, &QPushButton::pressed, this, &RegularExpressionDialog::reject);
	isreg = new QCheckBox("input will be explained in regular expression,if checked.");
	seclayout->addWidget(isreg);
	seclayout->addStretch();
	seclayout->addWidget(okbtn);
	seclayout->addWidget(calbtn);

	QVBoxLayout *vlayout = new QVBoxLayout;
	vlayout->addLayout(mainLayout);
	vlayout->addWidget(horizontalSeparator);
	vlayout->addLayout(seclayout);

	setLayout(vlayout);
}

QWidget *RegularExpressionDialog::setUpLeftUi()
{
	QWidget *container = new QWidget;

	QFormLayout *layout = new QFormLayout(container);
	layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	layout->setMargin(0);

	QLabel *regexpAndSubjectLabel = new QLabel(tr("<h3>Regular expression or text input</h3>"));
	layout->addRow(regexpAndSubjectLabel);

	patternLineEdit = new PatternLineEdit;
	patternLineEdit->setClearButtonEnabled(true);
	layout->addRow(tr("&Pattern:"), patternLineEdit);

	directoryLineEdit = new QLineEdit;
	QAction *browserAct = new QAction(QIcon(myApp()->qtAwesome()->icon(fa::folderopeno)), "browser", this);
	connect(browserAct, &QAction::triggered, [this] {
		QString directory =
			QFileDialog::getExistingDirectory(this, "Find files in...", ".");
		if (!directory.isEmpty()) {
			directoryLineEdit->setText(directory);
		}
	});
	directoryLineEdit->addAction(browserAct, QLineEdit::TrailingPosition);
	layout->addRow(tr("In folder:"), directoryLineEdit);
	filterLineEdit = new QLineEdit;
	filterLineEdit->setClearButtonEnabled(true);
	filterLineEdit->setPlaceholderText("support *,? multi-filters are spilted by comma");
	layout->addRow(tr("Name filter:"), filterLineEdit);
	rawStringLiteralLineEdit = new DisplayLineEdit;
	layout->addRow(tr("&Raw string literal:"), rawStringLiteralLineEdit);
	escapedPatternLineEdit = new DisplayLineEdit;
	layout->addRow(tr("&Escaped pattern:"), escapedPatternLineEdit);

	subjectTextEdit = new QPlainTextEdit;
	layout->addRow(tr("&Subject text:"), subjectTextEdit);

	caseInsensitiveOptionCheckBox = new QCheckBox(tr("Case insensitive (/i)"));
	dotMatchesEverythingOptionCheckBox = new QCheckBox(tr("Dot matches everything (/s)"));
	multilineOptionCheckBox = new QCheckBox(tr("Multiline (/m)"));
	extendedPatternSyntaxOptionCheckBox = new QCheckBox(tr("Extended pattern (/x)"));
	invertedGreedinessOptionCheckBox = new QCheckBox(tr("Inverted greediness"));
	dontCaptureOptionCheckBox = new QCheckBox(tr("Don't capture"));
	useUnicodePropertiesOptionCheckBox = new QCheckBox(tr("Use unicode properties (/u)"));
	optimizeOnFirstUsageOptionCheckBox = new QCheckBox(tr("Optimize on first usage"));
	dontAutomaticallyOptimizeOptionCheckBox = new QCheckBox(tr("Don't automatically optimize"));

	QGridLayout *patternOptionsCheckBoxLayout = new QGridLayout;
	int gridRow = 0;
	patternOptionsCheckBoxLayout->addWidget(caseInsensitiveOptionCheckBox, gridRow, 1);
	patternOptionsCheckBoxLayout->addWidget(dotMatchesEverythingOptionCheckBox, gridRow, 2);
	++gridRow;
	patternOptionsCheckBoxLayout->addWidget(multilineOptionCheckBox, gridRow, 1);
	patternOptionsCheckBoxLayout->addWidget(extendedPatternSyntaxOptionCheckBox, gridRow, 2);
	++gridRow;
	patternOptionsCheckBoxLayout->addWidget(invertedGreedinessOptionCheckBox, gridRow, 1);
	patternOptionsCheckBoxLayout->addWidget(dontCaptureOptionCheckBox, gridRow, 2);
	++gridRow;
	patternOptionsCheckBoxLayout->addWidget(useUnicodePropertiesOptionCheckBox, gridRow, 1);
	patternOptionsCheckBoxLayout->addWidget(optimizeOnFirstUsageOptionCheckBox, gridRow, 2);
	++gridRow;
	patternOptionsCheckBoxLayout->addWidget(dontAutomaticallyOptimizeOptionCheckBox, gridRow, 1);

	layout->addRow(tr("Pattern options:"), patternOptionsCheckBoxLayout);

	offsetSpinBox = new QSpinBox;
	layout->addRow(tr("Match &offset:"), offsetSpinBox);

	matchTypeComboBox = new QComboBox;
	matchTypeComboBox->addItem(tr("Normal"), QVariant::fromValue(QRegularExpression::NormalMatch));
	matchTypeComboBox->addItem(tr("Partial prefer complete"), QVariant::fromValue(QRegularExpression::PartialPreferCompleteMatch));
	matchTypeComboBox->addItem(tr("Partial prefer first"), QVariant::fromValue(QRegularExpression::PartialPreferFirstMatch));
	matchTypeComboBox->addItem(tr("No match"), QVariant::fromValue(QRegularExpression::NoMatch));
	layout->addRow(tr("Match &type:"), matchTypeComboBox);

	dontCheckSubjectStringMatchOptionCheckBox = new QCheckBox(tr("Don't check subject string"));
	anchoredMatchOptionCheckBox = new QCheckBox(tr("Anchored match"));

	QGridLayout *matchOptionsCheckBoxLayout = new QGridLayout;
	matchOptionsCheckBoxLayout->addWidget(dontCheckSubjectStringMatchOptionCheckBox, 0, 0);
	matchOptionsCheckBoxLayout->addWidget(anchoredMatchOptionCheckBox, 0, 1);
	layout->addRow(tr("Match options:"), matchOptionsCheckBoxLayout);

	return container;
}

QWidget *RegularExpressionDialog::setUpRightUi()
{
	QWidget *container = new QWidget;

	QVBoxLayout *layout = new QVBoxLayout(container);
	layout->setMargin(0);

	QLabel *matchInfoLabel = new QLabel(tr("<h3>Match information</h3>"));
	layout->addWidget(matchInfoLabel);

	matchDetailsTreeWidget = new QTreeWidget;
	matchDetailsTreeWidget->setHeaderLabels(QStringList() << tr("Occurrence") << tr("Group") << tr("Captured"));
	matchDetailsTreeWidget->setSizeAdjustPolicy(QTreeWidget::AdjustToContents);
	layout->addWidget(matchDetailsTreeWidget);

	QFrame *horizontalSeparator = new QFrame;
	horizontalSeparator->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	layout->addWidget(horizontalSeparator);

	QLabel *regexpInfoLabel = new QLabel(tr("<h3>Regular expression information</h3>"));
	layout->addWidget(regexpInfoLabel);

	regexpStatusLabel = new QLabel(tr("Valid"));
	regexpStatusLabel->setWordWrap(true);
	layout->addWidget(regexpStatusLabel);

	namedGroupsTreeWidget = new QTreeWidget;
	namedGroupsTreeWidget->setHeaderLabels(QStringList() << tr("Index") << tr("Name"));
	namedGroupsTreeWidget->setSizeAdjustPolicy(QTreeWidget::AdjustToContents);
	namedGroupsTreeWidget->setRootIsDecorated(false);
	layout->addWidget(namedGroupsTreeWidget);

	return container;
}

