#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QRadioButton>
namespace Ui {
class mainDialog;
}

class mainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit mainDialog(QWidget *parent = nullptr);
    ~mainDialog();
    using MODEL = enum{LOCAL,SERVER,CLIENT};
    bool isEnter();
    MODEL getModel();

private:
    MODEL _model;
    bool _isenter;
    Ui::mainDialog *ui;
    QPushButton *_enter, *_exit;
    QRadioButton *_local, *_server, *_client;
};

#endif // MAINDIALOG_H
