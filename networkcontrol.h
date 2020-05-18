#ifndef NETWORKCONTROL_H
#define NETWORKCONTROL_H

#include "ChessColorPro.h"
#include "boardmodelinterface.h"
#include "position.h"
#include "controlinterface.h"
#include <string>
#include <QTcpServer>
#include <QTcpSocket>
#include <remotecontrolinterface.h>
class BoardModelInterface;
class ControlInterface;
class NetworkControl:public ControlInterface,public RemoteControlInterface
{
public:
    NetworkControl();
    void putChessSignal(Position pos) override;
    void giveUpSignal(ChessColorPro ) override;
    void exitSignal(ChessColorPro) override;
    void timeOutSingal(ChessColorPro) override;
    void remotePutChessSignal(Position pos) override;
    void remoteGiveUpSignal() override;//共有
    void remoteExitSignal() override;//共有
    void remoteTimeOutSignal() override;//共有
    void remoteDisConnectSignal() override;
    void remoteBeginGameSignal() override;
    void show();
    ~NetworkControl();
private:
    using MESSAGE_FLAGS = enum{PW,PWOK,NAME,POS,EXIT,GIVEUP,TIMEOUT};
    using SERVER_STATE = enum{SW_PW,SW_NAME,SW_POS,SW_RUNING,SW_END} ;
    using  CLIENT_STATE = enum{CW_PWOK,CW_NAME,CW_POS,CW_RUNING,CW_END} ;
//    void initServerHandler();
//    void initClientHandler();
//    void readHanele(); //自行
//    void acceptHandle(); //自行
//    void disConnectHandle(); //自行
//    void writeHandle(char buf[],int len); //共有
//    void recvRW(char buf[],int len);//服务器专属
//    void recvPWOK(char buf[],int len);//客户端专属
//    //其余的地方都是Name
//    void recvNAME(char buf[],int len);//共有
//    //规定POS的规则是两个字节，
//    void recvPOS(char buf[],int len);//共有
//    void recvEXIT(char buf[],int len);//共有
//    void recvGIVEUP(char buf[],int len);//共有
//    void sendRW();//客户端专属
//    void sendPWOK();//服务器专属
//    void sendNAME(std::string name);//共有
//    void sendPOS(int x,int y);//共有
//    void sendEXIT();//共有
//    void sendGIVEUP();//共有
};

#endif // NETWORKCONTROL_H
