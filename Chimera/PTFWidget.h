#pragma once
#include <QWidget>
#include "ui_PTFWidget.h"

class PTFile {
public:
	PTFile(const QString &filename);
	QStringList GetPVNames() const { return PVNames; };
	QStringList GetPVDimensions() const { return PVDimensions; };
	QStringList GetPVInfo() const { return PVinfo; };
	QVector<int> GetPlotID() const { return PlotID; };
	QVector<int> GetPlotND() const { return PlotND; };
	int Getnkeyt() const { return nkeyt; };
	int Getnrect() const { return nrect; };
	QString Gettitle() const { return title; };
	QList<QVector<float>> GetPlot(const QVector<int> &idx);
private:
	qint32 nkeyt, nrect;
	QString title,filename;
	QStringList PVNames, PVDimensions, PVinfo;
	QVector<int> PlotID, PlotND;
	void init(QDataStream &stream);
};

class PTFWidget :
	public QWidget
{
	Q_OBJECT
public:
	PTFWidget();
	~PTFWidget();
	QString taskname()const;
	QString PTFfileName() const;
	void setFile(const QString &filename);
	static int count;
	bool isFold;
	void FoldOrExpand();
signals:
	void RequirePLOT(QString filepath, const QStringList &name,const QVector<int> &str);
private:
	Ui::PTFWidget ui;
	void SpecifiedList();
	void ShowMenu(const QPoint &pos);
	void PlotVars();
	QStringList pdlist;
	QVector<int> idxlist;
	QString filename;
	int nkeyt;
	int nrect;
};

