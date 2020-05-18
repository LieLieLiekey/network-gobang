#include "clientnetworkcontrol.h"



ClientNetworkControl::ClientNetworkControl()
{

}
void ClientNetworkControl::putChessSignal(Position pos)
{

}
void ClientNetworkControl::giveUpSignal(ChessColorPro )
{


}

void ClientNetworkControl::exitSignal(ChessColorPro)
{

}

void ClientNetworkControl::timeOutSingal(ChessColorPro)
{

}

void ClientNetworkControl::remotePutChessSignal(Position pos)
{

}

void ClientNetworkControl::remoteGiveUpSignal()
{

}

void ClientNetworkControl::remoteExitSignal()
{

}

void ClientNetworkControl::remoteTimeOutSignal()
{

}

void ClientNetworkControl::remoteDisConnectSignal()
{

}

void ClientNetworkControl::remoteBeginGameSignal()
{

}

void ClientNetworkControl::remotePasswdCurrect()
{

}

void ClientNetworkControl::start()
{

}

ClientNetworkControl::~ClientNetworkControl()
{

}
void ClientNetworkControl::DialogEnterHandle(ConnectDialog *dialog)
{

}

void ClientNetworkControl::DialogCancelHandle(ConnectDialog *dialog)
{

}

void ClientNetworkControl::DialogExitHandle(ConnectDialog *dialog)
{

}

void ClientNetworkControl::initDialog(ConnectDialog *dialog)
{

}

void ClientNetworkControl::initGame(QString selfname,QString remotename,int timeout = 600)
{

}

void ClientNetworkControl::exitHandle()
{

}

void ClientNetworkControl::gameOverHandle(ChessColorPro )
{

}

/*
 *     using  CLIENT_STATE = enum{CW_PWOK,CW_NAME,CW_POS,CW_RUNING,CW_END} ;
    CLIENT_STATE _state;
BoardModel * boardmodel;
Chesser *whiteer,*blacker;
BoardFrame * boardframe;
ShowInfoFrameUi *showinfo_ui;
QFrame *frame;
ClientSocketModel * clientmodel;
ConnectDialog * _dialog;
using END_FLAGS=enum{TIMEOUT,GAVEUP,END,NOEND};
END_FLAGS end_flag;
*/
