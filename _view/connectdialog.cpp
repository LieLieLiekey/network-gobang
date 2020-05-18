#include "connectdialog.h"
#include "ui_connectdialog.h"
/*
 QLineEdit _ipline,_portline,_nameline,_passwdline,_statusline;
 QPushButton _enter,_cancel,_exit;
*/
#include <QRegExp>
#include <QRegExpValidator>
ConnectDialog::ConnectDialog(QString title,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);
    _ipline =  ui->ip_line;
    _portline = ui->port_line;
    _nameline = ui->nick_line;
    _passwdline = ui->password_line;
    _enter = ui->enter_button;
    _cancel = ui->cancel_button;
    _exit = ui->exit_button;
    _statusinfo = ui->status_text;

//    _portline ->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$"),this));//正整数

//    _ipline ->setValidator(new QRegExpValidator(QRegExp(""),this));

//    _passwdline->setValidator(new QRegExpValidator(QRegExp(""),this));
}
void ConnectDialog::setRegExpValidator(QString reg,QLineEdit * line)
{
    line->setValidator(new QRegExpValidator(QRegExp(reg),line));
}
QLineEdit* ConnectDialog::getPortLineEdit()
{
    return _portline;
}

QLineEdit * ConnectDialog::getNameLineEdit()
{
    return _nameline;
}

QLineEdit * ConnectDialog::getPasswdLineEdit()
{
    return _passwdline;
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}
QString ConnectDialog::getIp()
{
    return _ipline->text();
}
int ConnectDialog::getPort()
{
   bool ok;
   int val = _portline->text().toUInt(&ok);
   if(ok == false) return -1;
   else return val;
}
QLineEdit* ConnectDialog::getIpLineEdit()
{
    return _ipline;
}
QString ConnectDialog::getName()
{
   return _nameline->text();
}
QString ConnectDialog::getPassword()
{
    return _passwdline->text();
}
void ConnectDialog::setStatus(QString info)
{
    _statusinfo->setText(info);
}
QPushButton* ConnectDialog::getEnterButton()
{
    return _enter;
}
QPushButton* ConnectDialog::getCancelButton()
{
    return _cancel;
}
QPushButton* ConnectDialog::getExitButton()
{
    return _exit;
}
