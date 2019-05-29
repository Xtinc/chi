#include "diffdialog.h"
#include "ui_diffdialog.h"
#include <QPushButton>
#include <QFileDialog>

DiffDialog::DiffDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiffDialog)
{
    ui->setupUi(this);
    connectLineAndBtn(ui->srcfile,ui->pushButton_3,file);
    connectLineAndBtn(ui->dstfile,ui->pushButton_4,file);
    connectLineAndBtn(ui->srcfolder,ui->pushButton,folder);
    connectLineAndBtn(ui->dstfloder,ui->pushButton_2,folder);
}

bool DiffDialog::isFile()
{
    return ui->filegroupBox->isChecked();
}

QString DiffDialog::ReturnSrc() const
{
    if(ui->filegroupBox->isChecked()&&(!ui->foldergroupBox->isChecked())){
        return ui->srcfile->text();
    }
    else if(ui->foldergroupBox->isChecked()&&(!ui->filegroupBox->isChecked())){
        return ui->srcfolder->text();
    }
    else{
        return QString();
    }
}

QString DiffDialog::ReturnDst() const
{
    if(ui->filegroupBox->isChecked()&&(!ui->foldergroupBox->isChecked())){
        return ui->dstfile->text();
    }
    else if(ui->foldergroupBox->isChecked()&&(!ui->filegroupBox->isChecked())){
        return ui->dstfloder->text();
    }
    else{
        return QString();
    }
}

QString DiffDialog::ReturnFliter() const
{
	if (ui->fliterLineEdit->text().isEmpty()) {
		return QStringLiteral("*.txt,*.dat");
	}
	else
	{
		return ui->fliterLineEdit->text();
	}
}

DiffDialog::~DiffDialog()
{
    delete ui;
}

void DiffDialog::on_foldergroupBox_toggled(bool arg1)
{
    ui->filegroupBox->setChecked(!arg1);
}

void DiffDialog::on_filegroupBox_toggled(bool arg1)
{
    ui->foldergroupBox->setChecked(!arg1);
}

void DiffDialog::connectLineAndBtn(QLineEdit *line, QPushButton *btn, ConnectType connecttype)
{
    connect(btn,&QPushButton::pressed,[line,connecttype]{
        switch (connecttype) {
        case file:
            line->setText(QFileDialog::getOpenFileName());
            break;
        case folder:
            line->setText(QFileDialog::getExistingDirectory());
            break;
        }
    });
}
