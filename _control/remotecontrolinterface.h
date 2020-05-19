#ifndef REMOTECONTROLINTERFACE_H
#define REMOTECONTROLINTERFACE_H
#include <position.h>
#include "ChessColorPro.h"
#include <QString>
class RemoteControlInterface
{
public:
    virtual void remotePutChessSignal(Position pos)=0;
    virtual void remoteGiveUpSignal()=0;//共有
    virtual void remoteExitSignal()=0;//共有
    virtual void remoteTimeOutSignal()=0;//共有
    virtual void remoteDisConnectSignal()=0;
    virtual void remoteBeginGameSignal()=0;
    virtual void remotePasswdCurrect()=0;
    using  EXCEPT_LEVEL = enum{NO,LOW,MID,HIGH,SERIOUS};
    virtual void errorHanle(EXCEPT_LEVEL level,QString errormsg);
    static constexpr ChessColorPro SERVER_COLOR = ChessColorPro::BLACK;
    static constexpr ChessColorPro CLIENT_COLOR = ChessColorPro::WHITE;
};

#endif // REMOTECONTROLINTERFACE_H
