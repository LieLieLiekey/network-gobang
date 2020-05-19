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
    void sendPOS(int x,int y)override;
    void sendEXIT()override;
    void sendGIVEUP()override;
    void sendPW();//发送pw和自己的名字
    void writeData(char buf[],int len)override;

    void recvMessage(char buf[],int len);
    void recvPOS(char buf[],int len);
    void recvEXIT(char buf[],int len);
    void recvGIVEUP(char buf[],int len);
    void recvPWOK(char buf[],int len);
    void readHanele(QTcpSocket *client); //自行
    void errorHandle();
    void close();
    void disConnectHandle(QTcpSocket *client);
    void connectHandle(QTcpSocket *client);
    bool start(QString name,QString passwd,int port,QString ip="localhost");
    QString getServerAddress();
    int getServerPort();
    ~ClientSocketModel();

    QString getRemoteName()override;
    bool alreadyBegin()override;

private:

    static constexpr int BUFSIZE = 256;
    QString _selfname,_remote_name,_passwd,_ip;
    int _port;
    RemoteControlInterface * _remote_control;
    QTcpSocket *_client;
    using STATE = enum{NO,READY,ACCPET,RECVPW,SENDPW,START,END};
    STATE _state;
};

#endif
