#pragma once
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter :
	public QSyntaxHighlighter
{
	Q_OBJECT
public:
	Highlighter(QTextDocument *parent = nullptr);
	~Highlighter();
protected:
	void highlightBlock(const QString &text) override;
private:
	struct HighlightingRule
	{
		QRegularExpression pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;

	QTextCharFormat keywordFormat;
	QTextCharFormat classFormat;
	QTextCharFormat commentFormat;
	QTextCharFormat plusFormat;
	QTextCharFormat minusFormat;
};

