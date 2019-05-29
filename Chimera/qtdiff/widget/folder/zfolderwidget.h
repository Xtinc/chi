#ifndef ZFOLDERWIDGET
#define ZFOLDERWIDGET

#include <QtWidgets>
#include "qtdiff/ctl/zfolderctl.h"
#include "qtdiff/diff/zpathdiff.h"
#include "qtdiff/diff/zpathdiffmodel.h"
#include "qtdiff/diff/zfilediff.h"
#include "qtdiff/diff/zfilediffmodel.h"
#include "QSpreadsheetHeaderView.h"

class ZFolderWidget : public QWidget{
    Q_OBJECT

public:
    ZFolderWidget(QWidget *parent = 0);
    ~ZFolderWidget();
	static int count;

public slots:
    void compare(const QString &srcBasePath, const QString &dstBasePath,const QString &fliter);
    void stopCompare();

signals:
    void fileCompare(ZPathDiffModel pathDiffModel);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

private:
    void initData();
    void initUI();
    void initConnect();
    void clearAll();
    void insert(const QList<ZTreeItemModel> &itemModelList);
    void updateInfo();

private slots:
    //void onSearchClicked();
    void onDiffMessage(const QList<ZTreeItemModel> &itemModelList);
    void onDiffEnd();
    void onProgress(int value, int maxValue);
    void onDoubleClicked(const QModelIndex & index);

private:
    int mFileAdded;
    int mFileRemoved;
    int mFileModified;
    int mLineAdded;
    int mLineRemoved;
    int mLineModified;
    QTime mTime;
    QString mFileInfo;
    QString mLineInfo;
    QString mTimeInfo;
    QLabel *mFileLabel;
    QLabel *mLineLabel;
    QLabel *mTimeLabel;
    QStringList mHeader;
	QSpreadsheetHeaderView *hv;
    QTableView *mTreeView;
    ZFolderCtl *mFolderCtl;
    QList<ZPathDiffModel> mPathModelLst;
    int mValue;
    int mMaxValue;
};

#endif // ZFOLDERWIDGET

