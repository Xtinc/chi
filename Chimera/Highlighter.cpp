#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
	:QSyntaxHighlighter(parent)
{
	HighlightingRule rule;
	keywordFormat.setFontWeight(QFont::Bold);
	rule.pattern = QRegularExpression("^\\[.*\\]", QRegularExpression::CaseInsensitiveOption);
	rule.format = keywordFormat;
	highlightingRules.append(rule);

	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::red);
	rule.pattern = QRegularExpression("(critical|fatal).*", QRegularExpression::CaseInsensitiveOption);
	rule.format = classFormat;
	highlightingRules.append(rule);

	commentFormat.setFontWeight(QFont::Bold);
	commentFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegularExpression("debug.*", QRegularExpression::CaseInsensitiveOption);
	rule.format = commentFormat;
	highlightingRules.append(rule);

	plusFormat.setForeground(Qt::darkYellow);
	rule.pattern = QRegularExpression("warning.*", QRegularExpression::CaseInsensitiveOption);
	rule.format = plusFormat;
	highlightingRules.append(rule);

	minusFormat.setFontWeight(QFont::Bold);
	rule.pattern = QRegularExpression("info", QRegularExpression::CaseInsensitiveOption);
	rule.format = minusFormat;
	highlightingRules.append(rule);
}

Highlighter::~Highlighter()
{
}

void Highlighter::highlightBlock(const QString & text)
{ 
	for each(const HighlightingRule &rule in highlightingRules) {
		QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
		while (matchIterator.hasNext()) {
			QRegularExpressionMatch match = matchIterator.next();
			setFormat(match.capturedStart(), match.capturedLength(), rule.format);
		}
	}
	setCurrentBlockState(0);
}
