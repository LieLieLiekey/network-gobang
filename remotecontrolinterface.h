#ifndef REMOTECONTROLINTERFACE_H
#define REMOTECONTROLINTERFACE_H
#include <position.h>
#include "ChessColorPro.h"
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
    static constexpr ChessColorPro SERVER_COLOR = ChessColorPro::BLACK;
    static constexpr ChessColorPro CLIENT_COLOR = ChessColorPro::WHITE;
};

#endif // REMOTECONTROLINTERFACE_H
