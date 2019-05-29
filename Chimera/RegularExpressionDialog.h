#pragma once
#include <QDialog>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QComboBox;
class QLabel;
class QSpinBox;
class QPlainTextEdit;
class QTreeWidget;
QT_END_NAMESPACE

class PatternLineEdit :public QLineEdit
{
	Q_OBJECT
public:
	explicit PatternLineEdit(QWidget *parent = nullptr);
private slots:
	void copyToCode();
	void pasteFromCode();
	void escapeSelection();
protected:
	void contextMenuEvent(QContextMenuEvent *event) override;
private:
	QAction *escapeSelectionAction;
	QAction *copyToCodeAction;
	QAction *pasteFromCodeAction;
};

class DisplayLineEdit : public QLineEdit {
public:
	explicit DisplayLineEdit(QWidget *parent = nullptr);
};

class RegularExpressionDialog : public QDialog
{
	Q_OBJECT
public:
	RegularExpressionDialog(QWidget *parent = nullptr);
	QString text() const;
	QString dir() const;
	QString filter() const;
	bool isREG() const;
private:
	void refresh();
	void setupUi();
	QWidget *setUpLeftUi();
	QWidget *setUpRightUi();
	void setResultUiEnabled(bool enabled);

	QLineEdit *patternLineEdit;
	QLineEdit *rawStringLiteralLineEdit;
	QLineEdit *escapedPatternLineEdit;
	QLineEdit *directoryLineEdit;
	QLineEdit *filterLineEdit;

	QPlainTextEdit *subjectTextEdit;

	QCheckBox *caseInsensitiveOptionCheckBox;
	QCheckBox *dotMatchesEverythingOptionCheckBox;
	QCheckBox *multilineOptionCheckBox;
	QCheckBox *extendedPatternSyntaxOptionCheckBox;
	QCheckBox *invertedGreedinessOptionCheckBox;
	QCheckBox *dontCaptureOptionCheckBox;
	QCheckBox *useUnicodePropertiesOptionCheckBox;
	QCheckBox *optimizeOnFirstUsageOptionCheckBox;
	QCheckBox *dontAutomaticallyOptimizeOptionCheckBox;
	QCheckBox *isreg;

	QSpinBox *offsetSpinBox;
	QComboBox *matchTypeComboBox;

	QCheckBox *anchoredMatchOptionCheckBox;
	QCheckBox *dontCheckSubjectStringMatchOptionCheckBox;

	QTreeWidget *matchDetailsTreeWidget;

	QLabel *regexpStatusLabel;
	QTreeWidget *namedGroupsTreeWidget;
};

