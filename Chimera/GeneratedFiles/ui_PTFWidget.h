/********************************************************************************
** Form generated from reading UI file 'PTFWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PTFWIDGET_H
#define UI_PTFWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PTFWidget
{
public:
    QGridLayout *gridLayout;
    QLabel *srclabel;
    QLabel *dstlabel;
    QFrame *line;
    QTreeWidget *treeWidget;
    QTreeWidget *treeWidget_2;

    void setupUi(QWidget *PTFWidget)
    {
        if (PTFWidget->objectName().isEmpty())
            PTFWidget->setObjectName(QString::fromUtf8("PTFWidget"));
        PTFWidget->resize(400, 300);
        gridLayout = new QGridLayout(PTFWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(0);
        srclabel = new QLabel(PTFWidget);
        srclabel->setObjectName(QString::fromUtf8("srclabel"));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(12);
        srclabel->setFont(font);

        gridLayout->addWidget(srclabel, 0, 1, 1, 1);

        dstlabel = new QLabel(PTFWidget);
        dstlabel->setObjectName(QString::fromUtf8("dstlabel"));
        dstlabel->setFont(font);
        dstlabel->setLayoutDirection(Qt::LeftToRight);
        dstlabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(dstlabel, 0, 2, 1, 1);

        line = new QFrame(PTFWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShadow(QFrame::Plain);
        line->setFrameShape(QFrame::HLine);

        gridLayout->addWidget(line, 1, 1, 1, 2);

        treeWidget = new QTreeWidget(PTFWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(1, QString::fromUtf8("Plot Units"));
        __qtreewidgetitem->setText(0, QString::fromUtf8("Plot Variables"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));
        treeWidget->setFrameShape(QFrame::NoFrame);
        treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeWidget->setColumnCount(2);

        gridLayout->addWidget(treeWidget, 2, 1, 1, 1);

        treeWidget_2 = new QTreeWidget(PTFWidget);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(1, QString::fromUtf8("FileInfo Value"));
        __qtreewidgetitem1->setText(0, QString::fromUtf8("FileInfo Key"));
        treeWidget_2->setHeaderItem(__qtreewidgetitem1);
        treeWidget_2->setObjectName(QString::fromUtf8("treeWidget_2"));
        treeWidget_2->setContextMenuPolicy(Qt::NoContextMenu);
        treeWidget_2->setFrameShape(QFrame::NoFrame);
        treeWidget_2->setColumnCount(2);

        gridLayout->addWidget(treeWidget_2, 2, 2, 1, 1);


        retranslateUi(PTFWidget);

        QMetaObject::connectSlotsByName(PTFWidget);
    } // setupUi

    void retranslateUi(QWidget *PTFWidget)
    {
        PTFWidget->setWindowTitle(QApplication::translate("PTFWidget", "Form", nullptr));
        srclabel->setText(QApplication::translate("PTFWidget", "PTF File Content", nullptr));
        dstlabel->setText(QApplication::translate("PTFWidget", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PTFWidget: public Ui_PTFWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PTFWIDGET_H
