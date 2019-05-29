#pragma once
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplottertools/jkqtptools.h"
#include <QWidget>
#include <QVector>

class GraphWidget :
	public QWidget
{
public:
	GraphWidget();
	~GraphWidget();
	static int count;
	void setData(const QVector<double> &X, const QVector<double> &Y);
	void setPTF(const QString &path,const QStringList &name,const QVector<int> &str);
private:
	JKQTPlotter *plotDate;
	void setPan(bool t);
};

