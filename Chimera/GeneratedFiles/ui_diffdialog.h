/********************************************************************************
** Form generated from reading UI file 'diffdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIFFDIALOG_H
#define UI_DIFFDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DiffDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *foldergroupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *srcfolder;
    QPushButton *pushButton;
    QLabel *label_2;
    QLineEdit *dstfloder;
    QPushButton *pushButton_2;
    QLabel *label_3;
    QLineEdit *fliterLineEdit;
    QGroupBox *filegroupBox;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLineEdit *srcfile;
    QPushButton *pushButton_3;
    QLabel *label_5;
    QLineEdit *dstfile;
    QPushButton *pushButton_4;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DiffDialog)
    {
        if (DiffDialog->objectName().isEmpty())
            DiffDialog->setObjectName(QString::fromUtf8("DiffDialog"));
        DiffDialog->resize(439, 253);
        verticalLayout = new QVBoxLayout(DiffDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        foldergroupBox = new QGroupBox(DiffDialog);
        foldergroupBox->setObjectName(QString::fromUtf8("foldergroupBox"));
        foldergroupBox->setCheckable(true);
        gridLayout = new QGridLayout(foldergroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(foldergroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        srcfolder = new QLineEdit(foldergroupBox);
        srcfolder->setObjectName(QString::fromUtf8("srcfolder"));
        srcfolder->setReadOnly(true);

        gridLayout->addWidget(srcfolder, 0, 1, 1, 1);

        pushButton = new QPushButton(foldergroupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(pushButton, 0, 2, 1, 1);

        label_2 = new QLabel(foldergroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        dstfloder = new QLineEdit(foldergroupBox);
        dstfloder->setObjectName(QString::fromUtf8("dstfloder"));
        dstfloder->setMinimumSize(QSize(200, 0));
        dstfloder->setReadOnly(true);

        gridLayout->addWidget(dstfloder, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(foldergroupBox);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMaximumSize(QSize(16777215, 16777215));

        gridLayout->addWidget(pushButton_2, 1, 2, 1, 1);

        label_3 = new QLabel(foldergroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        fliterLineEdit = new QLineEdit(foldergroupBox);
        fliterLineEdit->setObjectName(QString::fromUtf8("fliterLineEdit"));
        fliterLineEdit->setClearButtonEnabled(true);

        gridLayout->addWidget(fliterLineEdit, 2, 1, 1, 2);


        verticalLayout->addWidget(foldergroupBox);

        filegroupBox = new QGroupBox(DiffDialog);
        filegroupBox->setObjectName(QString::fromUtf8("filegroupBox"));
        filegroupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        filegroupBox->setFlat(false);
        filegroupBox->setCheckable(true);
        filegroupBox->setChecked(false);
        gridLayout_2 = new QGridLayout(filegroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_4 = new QLabel(filegroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        srcfile = new QLineEdit(filegroupBox);
        srcfile->setObjectName(QString::fromUtf8("srcfile"));

        gridLayout_2->addWidget(srcfile, 0, 1, 1, 1);

        pushButton_3 = new QPushButton(filegroupBox);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_2->addWidget(pushButton_3, 0, 2, 1, 1);

        label_5 = new QLabel(filegroupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 1, 0, 1, 1);

        dstfile = new QLineEdit(filegroupBox);
        dstfile->setObjectName(QString::fromUtf8("dstfile"));

        gridLayout_2->addWidget(dstfile, 1, 1, 1, 1);

        pushButton_4 = new QPushButton(filegroupBox);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_2->addWidget(pushButton_4, 1, 2, 1, 1);


        verticalLayout->addWidget(filegroupBox);

        verticalSpacer = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonBox = new QDialogButtonBox(DiffDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(DiffDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), DiffDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DiffDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DiffDialog);
    } // setupUi

    void retranslateUi(QDialog *DiffDialog)
    {
        DiffDialog->setWindowTitle(QApplication::translate("DiffDialog", "DiffDialog", nullptr));
        foldergroupBox->setTitle(QApplication::translate("DiffDialog", "Folder Diff", nullptr));
        label->setText(QApplication::translate("DiffDialog", "Source      folder:", nullptr));
        pushButton->setText(QApplication::translate("DiffDialog", "...", nullptr));
        label_2->setText(QApplication::translate("DiffDialog", "Destination folder:", nullptr));
        pushButton_2->setText(QApplication::translate("DiffDialog", "...", nullptr));
        label_3->setText(QApplication::translate("DiffDialog", "File       filters:", nullptr));
        fliterLineEdit->setPlaceholderText(QApplication::translate("DiffDialog", "*.txt,*.dat", nullptr));
        filegroupBox->setTitle(QApplication::translate("DiffDialog", "File Diff", nullptr));
        label_4->setText(QApplication::translate("DiffDialog", "Source           file:", nullptr));
        pushButton_3->setText(QApplication::translate("DiffDialog", "...", nullptr));
        label_5->setText(QApplication::translate("DiffDialog", "Destination      file:", nullptr));
        pushButton_4->setText(QApplication::translate("DiffDialog", "...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DiffDialog: public Ui_DiffDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIFFDIALOG_H
