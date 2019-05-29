#ifndef ZFILEWIDGET
#define ZFILEWIDGET

#include <QtWidgets>
#include "qtdiff/env/zcons.h"
#include "qtdiff/diff/zfilediff.h"
#include "qtdiff/diff/zfilediffmodel.h"
#include "qtdiff/diff/zpathdiff.h"
#include "qtdiff/diff/zpathdiffmodel.h"
#include "qtdiff/qdiffview/qdiffview.h"

class ZFileWidget : public QObject
{
    Q_OBJECT
public:
    ZFileWidget(ZPathDiffModel pathDiffModel, QWidget *parent = nullptr);
public slots:
    void compare();
private:
    ZPathDiffModel mPathDiffModel;
    QDiffView view;
};

#endif // ZFILEWIDGET

