#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QString>
namespace Ui {
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QString title,QWidget *parent = nullptr);
    QString getIp();
    int getPort();
    QString getName();
    QString getPassword();
    QLineEdit* getIpLineEdit();
    QLineEdit* getPortLineEdit();
    QLineEdit * getNameLineEdit();
    QLineEdit * getPasswdLineEdit();
    void setStatus(QString info);
    QPushButton *getEnterButton();
    QPushButton *getCancelButton();
    QPushButton *getExitButton();
    void setRegExpValidator(QString reg,QLineEdit * line);
    ~ConnectDialog();

private:
    Ui::ConnectDialog *ui;
    QLineEdit *_ipline,*_portline,*_nameline,*_passwdline;
    QTextEdit*_statusinfo;
    QPushButton *_enter,*_cancel,*_exit;
};

#endif // CONNECTDIALOG_H
