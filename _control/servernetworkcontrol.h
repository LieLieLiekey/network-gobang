#ifndef SERVERNETWORKCONTROL_H
#define SERVERNETWORKCONTROL_H
#include "ChessColorPro.h"
#include "boardmodelinterface.h"
#include "position.h"
#include "controlinterface.h"
#include <string>
#include <QTcpServer>
#include <QTcpSocket>
#include <remotecontrolinterface.h>
#include "connectdialog.h"
#include "boardmodel.h"
#include "boardframe.h"
#include "serversocketmodel.h"
class BoardModelInterface;
class ControlInterface;

class ServerNetworkControl:public ControlInterface,public RemoteControlInterface,public QObject
{
//    Q_OBJECT
public:
    ServerNetworkControl();
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
    ~ServerNetworkControl();
private:
    using SERVER_STATE = enum{SW_PW,SW_NAME,SW_POS,S_NOBEGIN,S_END,S_SELF} ;
    SERVER_STATE _state;
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
    ServerSocketModel * servermodel;
    ConnectDialog * _dialog;
    using END_FLAGS=enum{TIMEOUT,GAVEUP,END,NOEND};
    END_FLAGS end_flag;
};

#endif // SERVERNETWORKCONTROL_H