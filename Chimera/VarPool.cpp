#include "VarPool.h"

VarPool::VarPool()
{
}


VarPool::~VarPool()
{
}

void VarPool::insert(const QString & name, const QVector<float>& vec)
{
	Pool.insert(name, vec);
}

QVector<double> VarPool::FindByName(const QString & name)
{
	QMap<QString, QVector<float>>::Iterator it;
	QVector<float> tmp;
	it = Pool.find(name);
	if (it != Pool.end()) {
		tmp = it.value();
	}
	else
	{
		tmp.append(0.0);
	}
	QVector<double> pmt;
	for (auto i : tmp) {
		pmt.append(i);
	}
	return pmt;
}

QStringList VarPool::GetNameList()
{
	QStringList list;
	QMap<QString, QVector<float>>::Iterator  it;
	for (it = Pool.begin(); it != Pool.end(); ++it) {
		list << it.key();
	}
	return list;
}

void VarPool::clear()
{
	Pool.clear();
}
