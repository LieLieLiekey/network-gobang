#include "tcptest.h"
#include "ui_tcptest.h"
#include <QDebug>
TcpTest::TcpTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpTest)
{
    ui->setupUi(this);
    this->textView = ui->textView;
    this->exitbt = ui->exitbutton;
    QPushButton * pb2 = ui->nothing_button;
    connect(pb2,&QPushButton::clicked,[=]{
        while(true){
            qDebug() << "while True.\n";
        }
    });
    connect(this->exitbt,&QPushButton::clicked,[=]{

       this->close();

    });

}

TcpTest::~TcpTest()
{
    delete ui;
}
