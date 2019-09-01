/********************************************************************************
** Form generated from reading UI file 'ContentWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTENTWIDGET_H
#define UI_CONTENTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "TTable.h"

QT_BEGIN_NAMESPACE

class Ui_ContentWidget
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_3;
    QSpacerItem *horizontalSpacer_2;
    TTable *tableWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_3;
    QLabel *label;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_4;
    QListWidget *listWidget;
    QFrame *line_2;
    QFrame *line;
    QListView *VarView;
    QFrame *line_3;
    QPushButton *pushButton;

    void setupUi(QWidget *ContentWidget)
    {
        if (ContentWidget->objectName().isEmpty())
            ContentWidget->setObjectName(QString::fromUtf8("ContentWidget"));
        ContentWidget->resize(798, 561);
        ContentWidget->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(ContentWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(-1, -1, -1, 27);
        horizontalSpacer_3 = new QSpacerItem(122, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 2, 5, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 2, 0, 1, 1);

        tableWidget = new TTable(ContentWidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setFrameShape(QFrame::NoFrame);

        gridLayout->addWidget(tableWidget, 2, 3, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_3 = new QPushButton(ContentWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(12);
        pushButton_3->setFont(font);
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color:rgba(0, 0, 0, 0);\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgb(0, 0, 0,60);\n"
"}\n"
"QPushButton:pressed{\n"
"background-color:rgb(0,0,0,80);\n"
"}"));

        horizontalLayout->addWidget(pushButton_3);

        label = new QLabel(ContentWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Consolas"));
        font1.setPointSize(9);
        label->setFont(font1);

        horizontalLayout->addWidget(label);


        gridLayout->addLayout(horizontalLayout, 0, 3, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(ContentWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        gridLayout->addLayout(horizontalLayout_2, 0, 1, 1, 1);

        listWidget = new QListWidget(ContentWidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setContextMenuPolicy(Qt::NoContextMenu);
        listWidget->setStyleSheet(QString::fromUtf8("QListWidget::item\n"
"{\n"
"height:30px;\n"
"}\n"
"/*QListWidget::item::selected\n"
"{\n"
"background-color:rgb(0, 122, 204);\n"
"}*/"));
        listWidget->setFrameShape(QFrame::NoFrame);
        listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        gridLayout->addWidget(listWidget, 2, 1, 1, 1);

        line_2 = new QFrame(ContentWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_2, 2, 2, 1, 1);

        line = new QFrame(ContentWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 1, 1, 3);

        VarView = new QListView(ContentWidget);
        VarView->setObjectName(QString::fromUtf8("VarView"));
        VarView->setContextMenuPolicy(Qt::NoContextMenu);
        VarView->setFrameShape(QFrame::NoFrame);
        VarView->setFrameShadow(QFrame::Plain);
        VarView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        VarView->setSelectionBehavior(QAbstractItemView::SelectItems);

        gridLayout->addWidget(VarView, 2, 4, 1, 1);

        line_3 = new QFrame(ContentWidget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_3, 1, 4, 1, 1);

        pushButton = new QPushButton(ContentWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"background-color:rgba(0, 0, 0, 0);\n"
"}\n"
"QPushButton:hover{\n"
"background-color: rgb(0, 0, 0,60);\n"
"}\n"
"QPushButton:pressed{\n"
"background-color:rgb(0,0,0,80);\n"
"}"));

        gridLayout->addWidget(pushButton, 0, 4, 1, 1);

        gridLayout->setColumnStretch(0, 1);
        gridLayout->setColumnStretch(1, 2);
        gridLayout->setColumnStretch(2, 1);
        gridLayout->setColumnStretch(3, 6);
        gridLayout->setColumnStretch(4, 1);

        retranslateUi(ContentWidget);

        QMetaObject::connectSlotsByName(ContentWidget);
    } // setupUi

    void retranslateUi(QWidget *ContentWidget)
    {
        ContentWidget->setWindowTitle(QApplication::translate("ContentWidget", "Form", nullptr));
        pushButton_3->setText(QString());
        label->setText(QString());
        label_2->setText(QApplication::translate("ContentWidget", "Recent...", nullptr));
        pushButton->setText(QApplication::translate("ContentWidget", "Clear Vars", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ContentWidget: public Ui_ContentWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTENTWIDGET_H
