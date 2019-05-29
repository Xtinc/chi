#include "zfolderwidget.h"
#include "qtdiff/widget/tree/ztreemodel.h"
#include <QMetaType>
#include <QtMath>

int ZFolderWidget::count = 0;

ZFolderWidget::ZFolderWidget(QWidget *parent)
    : QWidget(parent)
    , mFolderCtl(NULL)
{
    initData();
    initUI();
    initConnect();
	count++;
}

ZFolderWidget::~ZFolderWidget()
{
    delete mFileLabel;
    delete mLineLabel;
    delete mTimeLabel;
    if(mTreeView->model() != nullptr)
    {
        ZTreeModel *model = (ZTreeModel *)mTreeView->model();
        delete model;
    }
    delete mTreeView;
    if(mFolderCtl != nullptr)
    {
        mFolderCtl->stopRunAndDelete();
    }
}


void ZFolderWidget::compare(const QString &srcBasePath,const QString &dstBasePath,const QString &fliter)
{
    mTime.restart();
    if(mFolderCtl != nullptr)
    {
        mFolderCtl->stopRunAndDelete();
    }
    clearAll();

    ZPathDiff pathDiff(srcBasePath, dstBasePath);
    pathDiff.setFilters(fliter);
    mPathModelLst = pathDiff.execute();

    mFolderCtl = new ZFolderCtl(srcBasePath, dstBasePath, mPathModelLst);
    connect(mFolderCtl, SIGNAL(diffMessage(const QList<ZTreeItemModel> &)), this, SLOT(onDiffMessage(const QList<ZTreeItemModel> &)), Qt::QueuedConnection);
    connect(mFolderCtl, SIGNAL(diffEnd()), this, SLOT(onDiffEnd()), Qt::QueuedConnection);
    connect(mFolderCtl, SIGNAL(progress(int,int)), this, SLOT(onProgress(int,int)), Qt::QueuedConnection);

    mFolderCtl->start();
}

void ZFolderWidget::stopCompare()
{
    if(mFolderCtl != nullptr)
    {
        mFolderCtl->stopRunAndDelete();
    }
    mTimeLabel->setText(mTimeInfo.arg(mTime.elapsed()/1000.0f));
}

void ZFolderWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    float progress = (float)((mValue * 1.0f) / (mMaxValue * 1.0f));
    if(progress < 1.0f)
    {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(QColor(3, 102, 214)));

        int width = qCeil(this->width() * progress);
        int height = 5;

        painter.drawRect(0, 0, width, height);
    }
}

void ZFolderWidget::initData()
{
    qRegisterMetaType< QList<ZTreeItemModel> >("QList<ZTreeItemModel>");

    mHeader << "#"
            << "Path"
            << "Extension"
            << "Status"
            << "Lines added"
            << "Lines removed"
            << "Lines modified";

    mValue = 1;
    mMaxValue = 1;

    mFileInfo = "% 10d files added,% 10d files removed,% 10d files modified,% 10d files total";
    mLineInfo = "% 10d lines added,% 10d lines removed,% 10d lines modified,% 10d lines total";
    mTimeInfo = "%1s elapsed";

    mFileAdded = 0;
    mFileRemoved = 0;
    mFileModified = 0;
    mLineAdded = 0;
    mLineRemoved = 0;
    mLineModified = 0;
}

void ZFolderWidget::initUI()
{
    mTreeView = new QTableView;
    ZTreeModel *model = new ZTreeModel(mHeader, "");
	hv = new QSpreadsheetHeaderView(Qt::Horizontal,this);
	mTreeView->setHorizontalHeader(hv);
    mTreeView->setModel(model);
    mTreeView->setAlternatingRowColors(false);
	mTreeView->setSelectionBehavior(QTableView::SelectRows);
	mTreeView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	mTreeView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    mFileLabel = new QLabel;
    mLineLabel = new QLabel;
    updateInfo();

    mTimeLabel = new QLabel;
    mTimeLabel->setText(mTimeInfo.arg(0));

    QVBoxLayout *folderLayout = new QVBoxLayout;
    QHBoxLayout *LIlayout=new QHBoxLayout;
    folderLayout->addWidget(mTreeView, 1);
    LIlayout->addWidget(mTimeLabel,0,Qt::AlignLeft);
    LIlayout->addWidget(mFileLabel,0,Qt::AlignRight);
    folderLayout->addLayout(LIlayout);
    folderLayout->addWidget(mLineLabel, 0, Qt::AlignRight);

    setLayout(folderLayout);

//    //test
//    mPathEditSrc->setText("E:/WorkSpace/QT/qt-diff_bak");
//    mPathEditDst->setText("E:/WorkSpace/QT/qt-diff");
}

void ZFolderWidget::initConnect()
{
    connect(mTreeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onDoubleClicked(QModelIndex)));
}

void ZFolderWidget::clearAll()
{
    QAbstractItemModel *model = mTreeView->model();
    if(model->rowCount() > 0)
    {
        model->removeRows(0, model->rowCount());
    }

    mFileAdded = 0;
    mFileRemoved = 0;
    mFileModified = 0;
    mLineAdded = 0;
    mLineRemoved = 0;
    mLineModified = 0;

    updateInfo();
}

void ZFolderWidget::insert(const QList<ZTreeItemModel> &itemModelList)
{
    QModelIndex index = mTreeView->model()->index(itemModelList[0].value().toInt() - 2, 0);
    QAbstractItemModel *model = mTreeView->model();

    if (!model->insertRow(index.row() + 1, index.parent()))
    {
        return;
    }

    int itemCount = itemModelList.size();
    for(int i = 0;i < itemCount;i++)
    {
        QModelIndex child = model->index(index.row() + 1, i, index.parent());
        model->setData(child, QVariant::fromValue(itemModelList[i]), Qt::DisplayRole);
    }

    QString status = itemModelList[3].value().toString();
    int i = 0;
    for(;i < (int)(sizeof(STATUS_STR) / sizeof(QString));i++)
    {
        if(QString::compare(status, STATUS_STR[i]) == 0)
        {
            break;
        }
    }
    switch((Status)i)
    {
    case Same:
        break;
    case Modified:
        mFileModified += 1;
        break;
    case Removed:
        mFileRemoved += 1;
        break;
    case Added:
        mFileAdded += 1;
        break;
    }
    mLineAdded += itemModelList[4].value().toInt();
    mLineRemoved += itemModelList[5].value().toInt();
    mLineModified += itemModelList[6].value().toInt();

    updateInfo();
}

void ZFolderWidget::updateInfo()
{
    QByteArray fileBa = mFileInfo.toLatin1();
    mFileLabel->setText(QString::asprintf(fileBa.data()
                        , mFileAdded
                        , mFileRemoved
                        , mFileModified
                        , mFileAdded + mFileRemoved + mFileModified));

    QByteArray lineBa = mLineInfo.toLatin1();
    mLineLabel->setText(QString::asprintf(lineBa.data()
                        , mLineAdded
                        , mLineRemoved
                        , mLineModified
                        , mLineAdded + mLineRemoved + mLineModified));
}

void ZFolderWidget::onDiffMessage(const QList<ZTreeItemModel> &itemModelList)
{
    insert(itemModelList);
}

void ZFolderWidget::onDiffEnd()
{
    QAbstractItemModel *model = mTreeView->model();
    for(int column = 0; column < model->columnCount(); ++column)
    {
        mTreeView->resizeColumnToContents(column);
    }

    if(mFolderCtl != NULL)
    {
        delete mFolderCtl;
        mFolderCtl = NULL;
    }

    mTimeLabel->setText(mTimeInfo.arg(mTime.elapsed()/1000.0f));
}

void ZFolderWidget::onProgress(int value, int maxValue)
{
    if(maxValue == 0)
    {
        mValue = 0;
        mMaxValue = 1;
    }
    else if(value > maxValue)
    {
        mValue = maxValue;
        mMaxValue = maxValue;
    }
    else
    {
        mValue = value;
        mMaxValue = maxValue;
    }
    update();
}

void ZFolderWidget::onDoubleClicked(const QModelIndex &index)
{
    int row = index.row();
    ZPathDiffModel pathDiffModel = mPathModelLst[row];
    emit fileCompare(pathDiffModel);
}
