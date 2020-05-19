#include "maindialog.h"
#include "ui_maindialog.h"
#include <QPushButton>
mainDialog::mainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainDialog)
{
    ui->setupUi(this);
    _enter = ui->enter_button;
    _exit = ui->exit_button;
    _server = ui->server_radio;
    _client = ui->client_radio;
    _local = ui->local_radio;
    _model = MODEL::LOCAL;
    _isenter = false;
    _local->setChecked(true);
    connect(_enter,&QPushButton::clicked,[&]{
      _isenter = true;
        close();
    });
    connect(_exit,&QPushButton::clicked,[&]{
      _isenter = false;
        close();
    });

    connect(_local,&QRadioButton::clicked,[&]{
      _model = MODEL::LOCAL;
    });
    connect(_server,&QRadioButton::clicked,[&]{
       _model = MODEL::SERVER;
    });

    connect(_client,&QRadioButton::clicked,[&]{
       _model = MODEL::CLIENT;
    });
}

mainDialog::~mainDialog()
{
    delete ui;
}

bool mainDialog::isEnter()
{
    return _isenter;
}
typename mainDialog::MODEL mainDialog::getModel()
{
       return _model;
}
