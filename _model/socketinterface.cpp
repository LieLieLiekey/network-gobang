#include "socketinterface.h"
#include <string.h>
#include <QHostAddress>
#include <QList>
#include <QNetworkInterface>
#include <QString>


SocketInterface::SocketInterface(RemoteControlInterface *remotecontrol)
{
    this->_remote_control =remotecontrol;
}
void SocketInterface::sendMessage(QString info)
{
    int len=0;
    send_buf[len++]=MESSAGE_FLAGS::MESSAGE;
    strcpy_s(send_buf + len,sendbuf_size-len,info.toStdString().c_str());
    len +=info.toStdString().length();
    writeData(send_buf,len);

}
void SocketInterface::sendPOS(int x,int y)
{

}
void SocketInterface::sendEXIT()
{

}
void SocketInterface::sendGIVEUP()
{

}
void sendPWOK(QString selfname);//发送pw和自己的名字
{

}
void sendPW(QString passwd,QString selfname)
{

}
int  readData(char buf[],int len)
{

}
 bool canReadData(char buf[],int len)
 {

 }

 void SocketInterface::recvMessage(char buf[],int len)
 {
     QString msg;
     for(int i=0;i<len;++i)
         msg.append(buf[i]);
     _remote_control->remoteMessageSignal(msg);
 }

 virtual void recvPOS(char buf[],int len)
 {

 }
 virtual void recvEXIT(char buf[],int len)
 {

 }
 virtual void recvGIVEUP(char buf[],int len)
 {

 }
 virtual void writeData(char buf[],int len)
 {

 }
/*
 virtual QString getRemoteName()=0;
virtual QString getServerAddress()=0;
 virtual QString getServerPort() =0;
 virtual bool alreadyBegin()=0;
 using MESSAGE_FLAGS = enum{PW,PWOK,POS,EXIT,GIVEUP,TIMEOUT,END,MESSAGE};
protected :
static constexpr int sendbuf_size=128;
char send_buf [sendbuf_size];
RemoteControlInterface *_remote_control;
*/

