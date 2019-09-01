#pragma once
#include <QMap>
#include <QVector>
#include <QString>
#include <QStringLIST>


class VarPool
{
public:
	VarPool();
	~VarPool();
	void insert(const QString &name, const QVector<float>&vec);
	QVector<double> FindByName(const QString &name);
	QStringList GetNameList();
	void clear();
private:
	QMap<QString, QVector<float>> Pool;
};

