#ifndef SERVERSOCKETMODEL_H
#define SERVERSOCKETMODEL_H
#include "socketinterface.h"
#include <QTcpServer>
#include <QTcpSocket>
#include "remotecontrolinterface.h"
class RemoteControlInterface;
class SocketInterface;
class ServerSocketModel:public SocketInterface,public QObject
{
public:
    ServerSocketModel(RemoteControlInterface *remote_control);
    virtual void sendPOS(int x,int y)override;
    virtual void sendEXIT()override;
    virtual void sendGIVEUP()override;
    void sendPWOK();//发送pw和自己的名字
    void writeData(char buf[],int len);

    void recvPOS(char buf[],int len);
    void recvEXIT(char buf[],int len);
    void recvGIVEUP(char buf[],int len);
    void recvPW(char buf[],int len);//服务器专属

    void readHanele(QTcpSocket *client); //自行
    void acceptHandle();
    void disConnectHandle(QTcpSocket *client);

    virtual QString getRemoteName()override;
    virtual bool alreadyBegin()override;

    void close();
    bool start(QString name,QString passwd,int port,QString ip="localhost");
    QString getServerAddress();
    int getServerPort();
    ~ServerSocketModel();
    static constexpr int BUFSIZE = 256;
    static constexpr int sendbuf_size=16;
     char send_buf [sendbuf_size];
private:
   QString _selfname,_remote_name,_passwd,_ip;
   int _port;
   RemoteControlInterface * _remote_control;
   QTcpServer * _server;
   QTcpSocket *_client;
   using STATE = enum{NO,READY,ACCPET,RECVPW,SENDPW,START,END};
   STATE _state;
/*
 accept
 recv pw name
 send pwok name
 begin game
*/
};

#endif // SERVERSOCKETMODEL_H

