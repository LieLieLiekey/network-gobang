#ifndef CLIENTSOCKETMODEL_H
#define CLIENTSOCKETMODEL_H

#include "socketinterface.h"
#include <QTcpServer>
#include <QTcpSocket>
#include "remotecontrolinterface.h"
class RemoteControlInterface;
class SocketInterface;

class ClientSocketModel:public SocketInterface,public QObject
{
public:
    ClientSocketModel(RemoteControlInterface * remote_control);

    void readHanele(QTcpSocket *client); //自行
    void errorHandle();
    void close();
    void disConnectHandle(QTcpSocket *client);
    void connectHandle(QTcpSocket *client);
    bool start(QString name,QString passwd,int port,QString ip="localhost");
    ~ClientSocketModel();
    QString getServerAddress() override;
    int getServerPort() override;
    QString getRemoteName()override;
    bool alreadyBegin()override;
    void sendPWOK(QString selfname) override;//发送pw和自己的名
    void sendPW(QString passwd,QString selfname)override;
    void recvPOS(char buf[],int len) override;
    void recvEXIT(char buf[],int len) override;
    void recvGIVEUP(char buf[],int len) override;
    virtual void recvPWOK(char buf[],int len)=0;
    virtual void recvPW(char buf[],int len)=0;
    void write(char buf[],int len) override;
private:
    static constexpr int BUFSIZE = 256;
    QString _selfname,_remote_name,_passwd,_ip;
    int _port;
    QTcpSocket *_client;
    using STATE = enum{NO,READY,ACCPET,RECVPW,SENDPW,START,END};
    STATE _state;
};
/*
    virtual void sendPOS(int x,int y);
    virtual void sendEXIT();
    virtual void sendGIVEUP();
    virtual void sendMessage(QString); //new add
    virtual void sendPWOK();//发送pw和自己的名字
    virtual void sendPWOK(QString selfname);//发送pw和自己的名
    virtual void sendPW(QString passwd,QString selfname);

    virtual void writeData(char buf[],int len); //将数据
    virtual int  readData(char buf[],int len) final; //分配数据 进行数据的分发
    virtual bool canReadData(char buf[],int len) final;

    virtual void write(char buf[],int len) =0;//将数据写到套接字内
    virtual void recvMessage(char buf[],int len);
    virtual void recvPOS(char buf[],int len);
    virtual void recvEXIT(char buf[],int len);
    virtual void recvGIVEUP(char buf[],int len);
    virtual void recvPWOK(char buf[],int len)=0;
    virtual void recvPW(char buf[],int len)=0;

*/
#endif
