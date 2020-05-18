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
    virtual QString getRemoteName()=0;
    virtual bool alreadyBegin()=0;
    using MESSAGE_FLAGS = enum{PW,PWOK,POS,EXIT,GIVEUP,TIMEOUT,END};
};

#endif // SOCKETINTERFACE_H
