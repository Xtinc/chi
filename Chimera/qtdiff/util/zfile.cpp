#include "zfile.h"
#include <QTextStream>
#include <QTextCodec>
#include <QFileIconProvider>
#include <QTemporaryFile>
#include "Application.h"
#include "zhash.h"
#include "Editor.h"
#include "QtAwesome/QtAwesome.h"

ZFile::ZFile()
{

}

ZFile::~ZFile()
{

}

int ZFile::lines(QFile *file)
{
    int count = -1;
    if(!file->open(QIODevice::ReadOnly))
    {
        return count;
    }
    else
    {
        count = 0;
    }

    QTextStream in(file);
	QTextCodec *codec_for_encoding = QTextCodec::codecForName(detectEncode(file->fileName()).toUtf8());
    in.setCodec(codec_for_encoding);
    while(!in.atEnd())
    {
        in.readLine();
        count++;
    }
    file->close();

    return count;
}

int ZFile::linesWithLine(QFile *file, QList<QString> &lineLst)
{
    int count = -1;
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return count;
    }
    else
    {
        count = 0;
    }

    QTextStream in(file);
	QTextCodec *codec_for_encoding = QTextCodec::codecForName(detectEncode(file->fileName()).toUtf8());
    in.setCodec(codec_for_encoding);
    while(!in.atEnd())
    {
        lineLst.append(in.readLine());
        count++;
    }
    file->close();

    return count;
}

int ZFile::linesWithHash(QFile *file, QList<unsigned int> &hashLst)
{
    int count = -1;
    if(!file->open(QIODevice::ReadOnly))
    {
        return count;
    }
    else
    {
        count = 0;
    }

    QTextStream in(file);
    QTextCodec *codec_for_encoding = QTextCodec::codecForName(detectEncode(file->fileName()).toUtf8());
    in.setCodec(codec_for_encoding);
    while(!in.atEnd())
    {
        QString line = in.readLine();
        hashLst.append(ZHash::ELFHash(line));
        count++;
    }
    file->close();

    return count;
}

int ZFile::linesWithHash(QFile * file, std::vector<unsigned int>& hashLst)
{
    int count = -1;
    if (!file->open(QIODevice::ReadOnly))
    {
        return count;
    }
    else
    {
        count = 0;
    }

    QTextStream in(file);
    QTextCodec *codec_for_encoding = QTextCodec::codecForName(detectEncode(file->fileName()).toUtf8());
    in.setCodec(codec_for_encoding);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        hashLst.push_back(ZHash::ELFHash(line));
        count++;
    }
    file->close();

    return count;
}

QFileInfoList ZFile::files(const QString &path, const QStringList &fliters)
{
    QFileInfoList fileList;
    if(!path.isEmpty())
    {
        QDir dir(path);
        fileList = dir.entryInfoList(fliters,QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        QFileInfoList folderList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

        for(int i = 0; i < folderList.size(); i++)
        {
             QString path = folderList.at(i).absoluteFilePath();
             QFileInfoList childFileList = files(path,fliters);
             fileList.append(childFileList);
        }
    }

    return fileList;
}

QIcon ZFile::icon(const QString &path)
{
    QFileIconProvider provider;
	QIcon icon;
    QTemporaryFile tmpFile(path);
    tmpFile.setAutoRemove(false);

    if (tmpFile.open())
    {
        tmpFile.close();
		QFileInfo fi(path);
		if (fi.exists()) {
			icon = provider.icon(QFileInfo(path));
		}
		else
		{
			icon = myApp()->qtAwesome()->icon(fa::exclamation);
		}
        tmpFile.remove();
    }

    return icon;
}
