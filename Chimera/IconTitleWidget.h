#pragma once
#include <QFrame>
class QIcon;
class QString;
class QLabel;

class IconTitleWidget : public QFrame
{
	Q_OBJECT

public:
	explicit IconTitleWidget(const QIcon& icon, const QString& title, QWidget *parent = 0);

public slots:
	void setIcon(const QIcon& icon);
	void setTitle(const QString& title);

public:
	QLabel* _iconLabel;
	QLabel* _titleLabel;
};

