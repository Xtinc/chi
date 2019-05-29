/********************************************************************************
** Form generated from reading UI file 'FindWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDWIDGET_H
#define UI_FINDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FindWidget
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *srclb;
    QSpacerItem *horizontalSpacer;
    QFrame *line;
    QTableWidget *tableWidget;

    void setupUi(QWidget *FindWidget)
    {
        if (FindWidget->objectName().isEmpty())
            FindWidget->setObjectName(QString::fromUtf8("FindWidget"));
        FindWidget->resize(256, 221);
        verticalLayout = new QVBoxLayout(FindWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(6, 6, 6, 6);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(FindWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(12);
        label->setFont(font);
        label->setContextMenuPolicy(Qt::NoContextMenu);

        horizontalLayout->addWidget(label);

        srclb = new QLabel(FindWidget);
        srclb->setObjectName(QString::fromUtf8("srclb"));
        srclb->setFont(font);
        srclb->setContextMenuPolicy(Qt::NoContextMenu);

        horizontalLayout->addWidget(srclb);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        line = new QFrame(FindWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShadow(QFrame::Plain);
        line->setFrameShape(QFrame::HLine);

        verticalLayout->addWidget(line);

        tableWidget = new QTableWidget(FindWidget);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Microsoft YaHei UI"));
        tableWidget->setFont(font1);
        tableWidget->setFrameShape(QFrame::NoFrame);
        tableWidget->setFrameShadow(QFrame::Sunken);
        tableWidget->setLineWidth(0);
        tableWidget->setColumnCount(2);

        verticalLayout->addWidget(tableWidget);


        retranslateUi(FindWidget);

        QMetaObject::connectSlotsByName(FindWidget);
    } // setupUi

    void retranslateUi(QWidget *FindWidget)
    {
        FindWidget->setWindowTitle(QApplication::translate("FindWidget", "Form", nullptr));
        label->setText(QApplication::translate("FindWidget", "Find Result:", nullptr));
        srclb->setText(QApplication::translate("FindWidget", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindWidget: public Ui_FindWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDWIDGET_H
