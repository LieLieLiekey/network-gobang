#ifndef CLIENTNETWORKCONTROL_H
#define CLIENTNETWORKCONTROL_H
#include "ChessColorPro.h"
#include "boardmodelinterface.h"
#include "position.h"
#include "controlinterface.h"
#include <string>
#include <QTcpServer>
#include <QTcpSocket>
#include <remotecontrolinterface.h>
#include "connectdialog.h"
#include "boardframe.h"
#include "showinfoframeui.h"
#include "clientsocketmodel.h"
class BoardModelInterface;
class ControlInterface;
class ClientNetworkControl:public ControlInterface,public RemoteControlInterface
{
public:
    ClientNetworkControl();
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
    void remotePasswdCurrect() override;
    void start();
    ~ClientNetworkControl();
private :
    using  CLIENT_STATE = enum{CW_PWOK,CW_NAME,CW_POS,CW_RUNING,CW_END} ;
    CLIENT_STATE _state;
    void DialogEnterHandle(ConnectDialog *dialog);
    void DialogCancelHandle(ConnectDialog *dialog);
    void DialogExitHandle(ConnectDialog *dialog);
    void initDialog(ConnectDialog *dialog);
    void initGame(QString selfname,QString remotename,int timeout = 600);
    void exitHandle();
    void gameOverHandle(ChessColorPro );
    BoardModel * boardmodel;
    Chesser *whiteer,*blacker;
    BoardFrame * boardframe;
    ShowInfoFrameUi *showinfo_ui;
    QFrame *frame;
    ClientSocketModel * clientmodel;
    ConnectDialog * _dialog;
    using END_FLAGS=enum{TIMEOUT,GAVEUP,END,NOEND};
    END_FLAGS end_flag;
};

#endif // CLIENTNETWORKCONTROL_H
