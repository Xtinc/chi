#pragma once
#include <QDialog>

namespace Ui {
	class melcorDig;
}

class melcorDig : public QDialog
{
	Q_OBJECT

public:
	explicit melcorDig(QWidget *parent = nullptr);
	QString FileName() const;
	QString WorkPath() const;
	~melcorDig();

private:
	Ui::melcorDig *ui;
	void OnFileBtnClicked();
	void OnDirBtnClicked();
};
