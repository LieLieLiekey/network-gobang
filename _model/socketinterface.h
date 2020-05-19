#ifndef SOCKETINTERFACE_H
#define SOCKETINTERFACE_H

#include <QString>
class SocketInterface
{
public:
    SocketInterface();
    virtual void sendPOS(int x,int y)=0;
    virtual void sendEXIT()=0;
    virtual void sendGIVEUP()=0;
    virtual void sendMessage(QString info); //new add
    virtual QString getRemoteName()=0;
    virtual bool alreadyBegin()=0;
    virtual void writeData(char buf[],int len)=0;
    using MESSAGE_FLAGS = enum{PW,PWOK,POS,EXIT,GIVEUP,TIMEOUT,END,MESSAGE};
protected:
    static constexpr int sendbuf_size=128;
    char send_buf [sendbuf_size];
};

#endif // SOCKETINTERFACE_H
