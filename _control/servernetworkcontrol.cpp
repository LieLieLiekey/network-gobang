#include "servernetworkcontrol.h"
#include <QTcpSocket>
#include <QMessageBox>
#include "chesserman.h"
ServerNetworkControl::ServerNetworkControl()
{
    _state = SERVER_STATE::S_NOBEGIN;
    end_flag = END_FLAGS::NOEND;
    servermodel = new ServerSocketModel(this);
    boardmodel = nullptr;
    whiteer= nullptr;
    blacker = nullptr;

    frame = nullptr;
    showinfo_ui = nullptr;
    boardframe = nullptr;
    _dialog = nullptr;

}
ServerNetworkControl::~ServerNetworkControl()
{
    if(frame !=nullptr)
        delete frame;
    if(boardmodel !=nullptr)
        delete boardmodel;
    if(whiteer !=nullptr)
        delete whiteer;
    if(blacker !=nullptr)
        delete blacker;
    if(servermodel!=nullptr)
        delete servermodel;
}
void ServerNetworkControl::putChessSignal(Position pos)
{
    static Position tmppos;
    if(boardmodel->getNowColor() == SERVER_COLOR)
    {
        boardmodel->putChess(pos,SERVER_COLOR);
        servermodel->sendPOS(pos.x,pos.y);/***********/
        _state = SERVER_STATE ::SW_POS;
        if(boardmodel ->is_gameOver())
        {
            end_flag=END_FLAGS::END;
            _state = SERVER_STATE::S_END;
            gameOverHandle(boardmodel->whoWin());
        }
    }
}

void ServerNetworkControl::giveUpSignal(ChessColorPro )
{
    QMessageBox msgBox;
    msgBox.setText("Warning ");
    msgBox.setInformativeText("Do you want to give up ?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::No:
          // Save was clicked
          break;
      case QMessageBox::Yes://放弃处理
        end_flag=END_FLAGS::GAVEUP;
        _state = SERVER_STATE::S_END;
        servermodel->sendGIVEUP();/***********/
        gameOverHandle(CLIENT_COLOR);
          break;
      default:
          // should never be reached
          break;
    }
}

void ServerNetworkControl::exitSignal(ChessColorPro)
{
    QMessageBox msgBox;
    msgBox.setText("Warning ");
    msgBox.setInformativeText("Do you want to exit ?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::No:
         //No
          break;
      case QMessageBox::Yes:
        //yes
        _state = SERVER_STATE::S_END;
        servermodel->sendEXIT();/***********/
        exitHandle();
          break;
      default:
          break;
    }
}

void ServerNetworkControl::timeOutSingal(ChessColorPro)
{
    QMessageBox msgBox;
    msgBox.setText("Warning ");
    msgBox.setInformativeText("You're too slow.");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.exec();
    _state = SERVER_STATE::S_END;
    end_flag=END_FLAGS::TIMEOUT;
    gameOverHandle(boardmodel->getNowColor()==SERVER_COLOR?CLIENT_COLOR:SERVER_COLOR);
}

void ServerNetworkControl::remotePutChessSignal(Position pos)
{
    static Position tmppos;
    if(boardmodel->getNowColor() == CLIENT_COLOR)
    {
        boardmodel->putChess(pos,CLIENT_COLOR);

        if(boardmodel ->is_gameOver())
        {
            end_flag=END_FLAGS::END;
            _state = SERVER_STATE::S_SELF;
            gameOverHandle(boardmodel->whoWin());
        }
    }
    else{
        errorHanle(EXCEPT_LEVEL::MID,"remote put chess positon error.");
    }
}

void ServerNetworkControl::remoteGiveUpSignal()
{
    end_flag = END_FLAGS::GAVEUP;
    _state = SERVER_STATE::S_END;
    gameOverHandle(SERVER_COLOR);
}

void ServerNetworkControl::remoteExitSignal()
{
    end_flag = END_FLAGS::GAVEUP;
    _state = SERVER_STATE::S_END;
    gameOverHandle(SERVER_COLOR);
}

void ServerNetworkControl::remoteTimeOutSignal()
/*暂时不会被调用*/
{
    end_flag = END_FLAGS::TIMEOUT;
    _state = SERVER_STATE::S_END;
    gameOverHandle(SERVER_COLOR);
}

void ServerNetworkControl::remoteDisConnectSignal()
{
    if(end_flag==END_FLAGS::NOEND)
        remoteExitSignal();
}

void ServerNetworkControl::remoteBeginGameSignal()
{
    _state = SERVER_STATE::S_SELF;
    _dialog->close();
}
void ServerNetworkControl::remotePasswdCurrect()
{
    //QString ip;
    _dialog->setStatus("connect success !\n ip:***,port:***,wait begin.");
}
void ServerNetworkControl::initDialog(ConnectDialog *dialog)
{
    connect(dialog->getEnterButton(),&QPushButton::clicked,[&]()
    {
        this->DialogEnterHandle(dialog);
    });
    connect(dialog->getCancelButton(),&QPushButton::clicked,[&]()
    {
        this->DialogCancelHandle(dialog);
    });
    connect(dialog->getExitButton(),&QPushButton::clicked,[&]()
    {
        this->DialogExitHandle(dialog);
    });
    QLineEdit * ipline = dialog->getIpLineEdit();
    ipline->setText(QString("localhost"));
    ipline->setDisabled(true);
    dialog->setRegExpValidator("^\\d+$",dialog->getPortLineEdit());//非负整数
    dialog->setRegExpValidator("^[A-Za-z0-9]+$",dialog->getPasswdLineEdit());//字母或数字
    dialog->setStatus(QString("no start. \nnotice:port must be integer,passwd must be integer or letter."));
    dialog->getEnterButton()->setEnabled(true);
    dialog->getCancelButton()->setDisabled(true);
}
void ServerNetworkControl::initGame(QString selfname,QString remotename,int timeout )
{
    /*设置棋手完毕*/
    blacker = new ChesserMan(selfname.toStdString(),SERVER_COLOR);
    whiteer = new ChesserMan(remotename.toStdString(),CLIENT_COLOR);
    boardmodel = new BoardModel(blacker,whiteer);

    boardframe = new BoardFrame(boardmodel,this,550,boardmodel->getBoardSize());
    showinfo_ui = new ShowInfoFrameUi(this,boardmodel,QString(blacker->getName().c_str()),QString(whiteer->getName().c_str()),timeout);

    /*初始化model和view完毕*/
    boardmodel->addObserver(boardframe);
    boardmodel->addObserver(showinfo_ui);
    /*将观察者 注册 到 主题完毕*/
    frame = new QFrame();
    frame->setFixedSize(850,550);
    boardframe->setParent(frame);
    showinfo_ui->setParent(frame);
    boardframe->move(0,0);
    showinfo_ui->move(550,0);
    /*UI窗口的组合完毕*/

    end_flag =END_FLAGS::NOEND;
    /*初始化完毕*/

    frame->show();
    /*显示窗体*/
}
void ServerNetworkControl::start()
{
    /**/
    _dialog = new ConnectDialog("Server - GoBang");
    initDialog(_dialog);
    _dialog -> exec() ;

    if(_state == SERVER_STATE::S_NOBEGIN)
    {
        /*fail*/
        delete  _dialog;
        _dialog = nullptr;
        return ;
    }
    initGame(_dialog->getName(),servermodel ->getRemoteName());
    delete  _dialog;
    _dialog = nullptr;
    //开始newChesser空间等
}
/*
 using SERVER_STATE = enum{SW_PW,SW_NAME,SW_POS,S_NOBEGIN,S_END,S_SELF} ;
*/

void ServerNetworkControl::DialogEnterHandle(ConnectDialog *dialog)
{
    /*缺少输入框禁止输入：*/
    int port = dialog->getPort();
    QString name = dialog->getName();
    QString passwd = dialog->getPassword();
    if(port == -1 || name ==""||passwd =="")
    {
        QMessageBox mesbox;
        mesbox.setText("port must be integer.\npasswd must be integer or letter.\npassworrd can't empty.");
        mesbox.exec();
        return ;
    }
    /*
    进行服务器的配置和连接，注册信号。
    */
    if(!servermodel ->start(name,passwd,port))
    {
        QMessageBox mesbox;
        mesbox.setText("port error! The port may be occupied\n");
        mesbox.exec();
        return ;
    }
    dialog->getEnterButton()->setEnabled(false);
    dialog->getCancelButton()->setEnabled(true);

    dialog->getPortLineEdit()->setEnabled(false);
    dialog->getNameLineEdit()->setEnabled(false);
    dialog->getPasswdLineEdit()->setEnabled(false);

    dialog->setStatus("server waiting for connect.ip:"+servermodel->getServerAddress()+"  port:" + QString::number(servermodel->getServerPort()));
}

void ServerNetworkControl::DialogCancelHandle(ConnectDialog *dialog)
{
    dialog->getEnterButton()->setEnabled(true);
    dialog->getCancelButton()->setEnabled(false);

    dialog->getPortLineEdit()->setEnabled(true);
    dialog->getNameLineEdit()->setEnabled(true);
    dialog->getPasswdLineEdit()->setEnabled(true);

     servermodel ->close();
     dialog->setStatus(QString("no start. \nnotice:port must be integer,passwd must be integer or letter."));

}

void ServerNetworkControl::DialogExitHandle(ConnectDialog *dialog)
{
    /*close*/
    if(servermodel !=nullptr )
    {
        delete servermodel ;
        servermodel =nullptr;
    }
    if(dialog!=nullptr)
    {
        dialog->close();
    }
}
void ServerNetworkControl::exitHandle()
{
    frame->close();
}

void ServerNetworkControl::gameOverHandle(ChessColorPro who)
{
    QString infoname;
    if(end_flag==END_FLAGS::END)
    {
        if(boardmodel->is_DogFall()){
               infoname = "Dogfall.";
        }
        else if(who == ChessColorPro::BLACK){
             infoname = QString(blacker->getName().c_str()) + "  ( BLACK ) is  Winer!";
        }
        else{
            infoname = QString(whiteer->getName().c_str()) + "  ( WHITE ) is  Winer!";
        }
    }
    else if(end_flag==END_FLAGS::GAVEUP)
    {
        if(whiteer->getColor()==who)//black loser
        {
            infoname = QString(blacker->getName().c_str()) + " is Loser!";
        }
        else{//white loser
            infoname = QString(whiteer->getName().c_str()) + " is Loser!";
        }

    }
    else if(end_flag==END_FLAGS::TIMEOUT)
    {

        if(whiteer->getColor()==who)//black loser
        {
            infoname = QString(whiteer->getName().c_str()) + "  ( WHITE ) is  Winer!";
        }
        else{//white loser
            infoname = QString(blacker->getName().c_str()) + "  ( BLACK ) is  Winer!";
        }
        //另一方胜利
    }
    else{
        infoname = "error.\n";
    }
    QMessageBox msgBox;
    msgBox.setText("Game over");
    msgBox.setInformativeText(infoname);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.exec();

    exitHandle();
}
