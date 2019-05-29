/********************************************************************************
** Form generated from reading UI file 'MelDig.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MELDIG_H
#define UI_MELDIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_melcorDig
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_2;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *melcorDig)
    {
        if (melcorDig->objectName().isEmpty())
            melcorDig->setObjectName(QString::fromUtf8("melcorDig"));
        melcorDig->resize(280, 99);
        gridLayout = new QGridLayout(melcorDig);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(melcorDig);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit = new QLineEdit(melcorDig);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setReadOnly(true);

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);

        pushButton = new QPushButton(melcorDig);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 0, 2, 1, 1);

        label_2 = new QLabel(melcorDig);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_2 = new QLineEdit(melcorDig);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setReadOnly(true);

        gridLayout->addWidget(lineEdit_2, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(melcorDig);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 1, 2, 1, 1);

        buttonBox = new QDialogButtonBox(melcorDig);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 3);


        retranslateUi(melcorDig);
        QObject::connect(buttonBox, SIGNAL(accepted()), melcorDig, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), melcorDig, SLOT(reject()));

        QMetaObject::connectSlotsByName(melcorDig);
    } // setupUi

    void retranslateUi(QDialog *melcorDig)
    {
        melcorDig->setWindowTitle(QApplication::translate("melcorDig", "Choose an inputdeck", nullptr));
        label->setText(QApplication::translate("melcorDig", "Input:", nullptr));
        lineEdit->setPlaceholderText(QApplication::translate("melcorDig", "choose a input file.", nullptr));
        pushButton->setText(QApplication::translate("melcorDig", "browser", nullptr));
        label_2->setText(QApplication::translate("melcorDig", "Output:", nullptr));
        lineEdit_2->setPlaceholderText(QApplication::translate("melcorDig", "choose output directory.", nullptr));
        pushButton_2->setText(QApplication::translate("melcorDig", "browser", nullptr));
    } // retranslateUi

};

namespace Ui {
    class melcorDig: public Ui_melcorDig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MELDIG_H
