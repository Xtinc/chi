#include "melcordig.h"
#include "ui_MelDig.h"
#include <QFileDialog>
#include <QPushButton>
#include <QSize>

melcorDig::melcorDig(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::melcorDig)
{
	ui->setupUi(this);
	connect(ui->pushButton, &QPushButton::clicked, this, &melcorDig::OnFileBtnClicked);
	connect(ui->pushButton_2, &QPushButton::clicked, this, &melcorDig::OnDirBtnClicked);
	setFixedSize(sizeHint());
}

QString melcorDig::FileName() const
{
	return ui->lineEdit->text();
}

QString melcorDig::WorkPath() const
{
	return ui->lineEdit_2->text();
}

melcorDig::~melcorDig()
{
	delete ui;
}

void melcorDig::OnFileBtnClicked()
{
	QString filename = QFileDialog::getOpenFileName(this, "choose input files.", ".");
	if (!filename.isEmpty()) {
		ui->lineEdit->setText(filename);
	}
}

void melcorDig::OnDirBtnClicked()
{
	QString directory = QFileDialog::getExistingDirectory(this, "Find files in...", ".");
	if (!directory.isEmpty()) {
		ui->lineEdit_2->setText(directory);
	}
}
