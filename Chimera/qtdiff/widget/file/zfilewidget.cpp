#include "zfilewidget.h"
#include "qtdiff/util/zfile.h"
#include "qtdiff/util/zrand.h"
#include <QFile>
#include <QHBoxLayout>

ZFileWidget::ZFileWidget(ZPathDiffModel pathDiffModel, QWidget *parent)
    : QObject(parent)
    , mPathDiffModel(pathDiffModel)
{
}

void ZFileWidget::compare()
{
    QString srcPath = mPathDiffModel.srcFileInfo().absoluteFilePath();
    QString dstPath = mPathDiffModel.dstFileInfo().absoluteFilePath();

    if(!srcPath.isEmpty()){
        QFile file(srcPath);
        file.open(QFile::ReadOnly);
        QTextStream in(&file);
        srcPath=in.readAll();
        file.close();
    }
    if(!dstPath.isEmpty()){
        QFile file2(dstPath);
        file2.open(QFile::ReadOnly);
        QTextStream in2(&file2);
        dstPath=in2.readAll();
        file2.close();
    }
    view.setSource(srcPath,dstPath);
}
