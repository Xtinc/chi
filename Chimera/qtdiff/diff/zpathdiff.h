#ifndef ZPATHDIFF
#define ZPATHDIFF

#include <QString>
#include <QFile>
#include "zpathdiffmodel.h"

class ZPathDiff {

public:
    ZPathDiff();
    ZPathDiff(const QString &pathSrc, const QString &pathDst);
    ~ZPathDiff();

    void setPathSrc(const QString &pathSrc);
    void setPathDst(const QString &pathDst);
    void setFilters(const QString &fliter);
    QStringList GetFilters() const;

    QList<ZPathDiffModel> execute();

private:
    void sort(QList<ZPathDiffModel> &pathModelList);

private:
    QString mPathSrc;
    QString mPathDst;
    QStringList list;
};

#endif // ZPATHDIFF

