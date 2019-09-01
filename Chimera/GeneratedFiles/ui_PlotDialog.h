/********************************************************************************
** Form generated from reading UI file 'PlotDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTDIALOG_H
#define UI_PLOTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PlotDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QComboBox *typcmb;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QLabel *label_3;
    QComboBox *xcmb;
    QLabel *label_4;
    QListView *ylistView;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PlotDialog)
    {
        if (PlotDialog->objectName().isEmpty())
            PlotDialog->setObjectName(QString::fromUtf8("PlotDialog"));
        PlotDialog->resize(480, 640);
        verticalLayout = new QVBoxLayout(PlotDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox = new QGroupBox(PlotDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        typcmb = new QComboBox(groupBox);
        typcmb->addItem(QString());
        typcmb->addItem(QString());
        typcmb->addItem(QString());
        typcmb->setObjectName(QString::fromUtf8("typcmb"));

        gridLayout->addWidget(typcmb, 1, 1, 1, 1);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(PlotDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        xcmb = new QComboBox(groupBox_2);
        xcmb->setObjectName(QString::fromUtf8("xcmb"));

        gridLayout_2->addWidget(xcmb, 1, 0, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        ylistView = new QListView(groupBox_2);
        ylistView->setObjectName(QString::fromUtf8("ylistView"));

        gridLayout_2->addWidget(ylistView, 3, 0, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        buttonBox = new QDialogButtonBox(PlotDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(PlotDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PlotDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PlotDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(PlotDialog);
    } // setupUi

    void retranslateUi(QDialog *PlotDialog)
    {
        PlotDialog->setWindowTitle(QApplication::translate("PlotDialog", "Dialog", nullptr));
        groupBox->setTitle(QApplication::translate("PlotDialog", "Basic Settings", nullptr));
        label->setText(QApplication::translate("PlotDialog", "Graph Title:", nullptr));
        label_2->setText(QApplication::translate("PlotDialog", "Graph Type:", nullptr));
        typcmb->setItemText(0, QApplication::translate("PlotDialog", "Line Chart", nullptr));
        typcmb->setItemText(1, QApplication::translate("PlotDialog", "Spline Chart", nullptr));
        typcmb->setItemText(2, QApplication::translate("PlotDialog", "Scatter Chart", nullptr));

        groupBox_2->setTitle(QApplication::translate("PlotDialog", "Variables", nullptr));
        label_3->setText(QApplication::translate("PlotDialog", "Select a variables as X", nullptr));
        label_4->setText(QApplication::translate("PlotDialog", "Select a variables as Y", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PlotDialog: public Ui_PlotDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTDIALOG_H
