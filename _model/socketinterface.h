#ifndef SOCKETINTERFACE_H
#define SOCKETINTERFACE_H
#include "remotecontrolinterface.h"
#include <QString>
class SocketInterface
{
/*
- 当有数据读时，调用canReadDat判断是否当作一次数据，
如果可以，调用readData，会自动的将数据进行分发。
当调用send时，会自动调用write函数
所以必须
1.读取数据时，调用can，再readData
2.写数据时，调用write函数，该函数必须由自己实现。
3.send函数自动调用write函数
4.reacv函数需要自己实现，某些实现的有钩子。
*/
public:
    SocketInterface(RemoteControlInterface * remote_control);
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

    virtual QString getRemoteName()=0;
   virtual QString getServerAddress()=0;
    virtual int getServerPort() =0;
    virtual bool alreadyBegin()=0;
    using MESSAGE_FLAGS = enum{PW,PWOK,POS,EXIT,GIVEUP,TIMEOUT,END,MESSAGE};
protected :
   static constexpr int sendbuf_size=128;
   char send_buf [sendbuf_size];
   RemoteControlInterface *_remote_control;
};

#endif // SOCKETINTERFACE_H
