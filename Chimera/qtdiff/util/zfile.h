#ifndef ZFILE
#define ZFILE

#include <QFile>
#include <QList>
#include <QFileInfo>
#include <QDir>
#include <QIcon>

class ZFile{
public:
    ZFile();
    ~ZFile();

    static int lines(QFile *file);

    static int linesWithLine(QFile *file, QList<QString> &lineLst);

    static int linesWithHash(QFile *file, QList<unsigned int> &hashLst);
    static int linesWithHash(QFile *file, std::vector<unsigned int> &hashLst);

    static QFileInfoList files(const QString &path,const QStringList &fliters);

    static QIcon icon(const QString &path);
};

#endif // ZFILE

