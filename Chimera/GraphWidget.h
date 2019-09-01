#pragma once
#include "jkqtplotter/jkqtplotter.h"
#include "jkqtplottertools/jkqtptools.h"
#include <QWidget>
#include <QVector>

class GraphWidget :
	public QWidget
{
public:
	enum ChartType {
		Line,
		Spline,
		Scatter
	};
	GraphWidget();
	~GraphWidget();
	static int count;
	void setData(const QString &Title,const QString &NameX,const QVector<double> &X, const QStringList &NameY,const QList<QVector<double>> &Y,ChartType ctype);
	void setPTF(const QString &path,const QStringList &name,const QVector<int> &str);
private:
	JKQTPlotter *plotDate;
	void setPan(bool t);
};

