#ifndef DIFFDIALOG_H
#define DIFFDIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class DiffDialog;
}

class DiffDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DiffDialog(QWidget *parent = nullptr);
    bool isFile();
    QString ReturnSrc() const;
    QString ReturnDst() const;
    QString ReturnFliter() const;
    ~DiffDialog();

private slots:
    void on_foldergroupBox_toggled(bool arg1);

    void on_filegroupBox_toggled(bool arg1);

private:
    Ui::DiffDialog *ui;
    enum ConnectType{
        file,
        folder
    };
    void connectLineAndBtn(QLineEdit *line,QPushButton *btn,ConnectType connecttype);
};

#endif // DIFFDIALOG_H
