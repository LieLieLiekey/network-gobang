#include "clientnetworkcontrol.h"


#include <QMessageBox>
/*
不定式：
exit之后，界面关闭，但并不会析构
结束游戏后，state为END
未开始猴戏时，state未NOBEGIN
*/
ClientNetworkControl::ClientNetworkControl()
{
    _state = CLIENT_STATE::C_NOBEGIN;
    end_flag = END_FLAGS::NOSTART;
    clientmodel = new ClientSocketModel(this);
    boardmodel = nullptr;
    whiteer= nullptr;
    blacker = nullptr;

    frame = nullptr;
    showinfo_ui = nullptr;
    boardframe = nullptr;
    _dialog = nullptr;
    chatframe = nullptr;
}

ClientNetworkControl::~ClientNetworkControl()
{

    if(frame !=nullptr)
        delete frame;
    if(boardmodel !=nullptr)
        delete boardmodel;
    if(whiteer !=nullptr)
        delete whiteer;
    if(blacker !=nullptr)
        delete blacker;
    if(clientmodel!=nullptr)
        delete clientmodel;
}
void ClientNetworkControl::start()
{
    _dialog = new ConnectDialog("Client - GoBang");
    initDialog(_dialog);
    _dialog -> exec() ;

    if(_state == CLIENT_STATE::C_NOBEGIN)
    {
        /*fail*/
        delete  _dialog;
        _dialog = nullptr;
        return ;
    }
    initGame(_dialog->getName(),clientmodel ->getRemoteName());
    delete  _dialog;
    _dialog = nullptr;
    //开始newChesser空间等
}
void ClientNetworkControl::putChessSignal(Position pos)
{

    static Position tmppos;
    if(boardmodel->getNowColor() == CLIENT_COLOR)
    {
        boardmodel->putChess(pos,CLIENT_COLOR);
        clientmodel->sendPOS(pos.x,pos.y);/***********/
        _state = CLIENT_STATE::CW_POS;
        if(boardmodel ->is_gameOver())
        {
            end_flag=END_FLAGS::END;
            _state = CLIENT_STATE::C_END;
            gameOverHandle(boardmodel->whoWin());
        }
    }
}
void ClientNetworkControl::giveUpSignal(ChessColorPro )
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
        _state = CLIENT_STATE::C_END;
        clientmodel->sendGIVEUP();/***********/
        gameOverHandle(CLIENT_COLOR);
          break;
      default:
          // should never be reached
          break;
    }
}

void ClientNetworkControl::exitSignal(ChessColorPro)
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
        _state = CLIENT_STATE::C_END;
        end_flag = END_FLAGS::GAVEUP;
        clientmodel->sendEXIT();/***********/
        exitHandle();
          break;
      default:
          break;
    }

}

void ClientNetworkControl::timeOutSingal(ChessColorPro)
{
    QMessageBox msgBox;
    msgBox.setText("Warning ");
    msgBox.setInformativeText("You're too slow.");
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.exec();
    _state = CLIENT_STATE::C_END;
    end_flag=END_FLAGS::TIMEOUT;
    gameOverHandle(boardmodel->getNowColor()==SERVER_COLOR?CLIENT_COLOR:SERVER_COLOR);
}

void ClientNetworkControl::remotePutChessSignal(Position pos)
{
    static Position tmppos;
    if(boardmodel->getNowColor() == SERVER_COLOR)
    {
        boardmodel->putChess(pos,SERVER_COLOR);

        if(boardmodel ->is_gameOver())
        {
            end_flag=END_FLAGS::END;
            _state = CLIENT_STATE::C_SELF;
            gameOverHandle(boardmodel->whoWin());
        }
    }
    else{
        errorHanle(EXCEPT_LEVEL::MID,"remote put chess positon error.");
    }

}

void ClientNetworkControl::remoteGiveUpSignal()
{
    end_flag = END_FLAGS::GAVEUP;
    _state = CLIENT_STATE::C_END;
    gameOverHandle(CLIENT_COLOR);//自己胜利
}

void ClientNetworkControl::remoteExitSignal()
{
    end_flag = END_FLAGS::GAVEUP;
    _state = CLIENT_STATE::C_END;
    gameOverHandle(CLIENT_COLOR);//自己胜利
}

void ClientNetworkControl::remoteTimeOutSignal()
/*暂时不会被调用*/
{
    end_flag = END_FLAGS::TIMEOUT;
    _state = CLIENT_STATE::C_END;
    gameOverHandle(CLIENT_COLOR);//自己胜利
}

void ClientNetworkControl::remoteDisConnectSignal()
{
    if(end_flag==END_FLAGS::RUN)
        remoteExitSignal();

}
void ClientNetworkControl::remoteMessageSignal(QString info)
{
    if(chatframe == nullptr){
        errorHanle(EXCEPT_LEVEL::LOW,"没有开始,但是收到了Message.");
    }
    else{
        chatframe ->appendMessage(clientmodel->getRemoteName(),info);
    }
}
void ClientNetworkControl::remoteBeginGameSignal()
{
    _state = CLIENT_STATE::C_SELF;
    _dialog->close();
}

void ClientNetworkControl::remotePasswdCurrect()
{
        _dialog->setStatus("connect server success !\n ip:***,port:***,wait begin.");
}
void ClientNetworkControl::sendMessagehandle()
{
    /*不忽略为空*/
    QString msg= chatframe->getLineInfo();
    chatframe->appendMessage("self",msg);
    chatframe->clearlineInfo();
    if(clientmodel->alreadyBegin())
        clientmodel->sendMessage(msg);
    else{
        errorHanle(EXCEPT_LEVEL::LOW,"send message error.");
    }
}
void ClientNetworkControl::DialogEnterHandle(ConnectDialog *dialog)
{
        QString ip = dialog->getIp();
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
        if(!clientmodel ->start(name,passwd,port,ip))
        {
            QMessageBox mesbox;
            mesbox.setText("port error! The port may be occupied\n");
            mesbox.exec();
            return ;
        }
        dialog->getEnterButton()->setEnabled(false);
        dialog->getCancelButton()->setEnabled(true);

        dialog ->getIpLineEdit()->setEnabled(false);
        dialog->getPortLineEdit()->setEnabled(false);
        dialog->getNameLineEdit()->setEnabled(false);
        dialog->getPasswdLineEdit()->setEnabled(false);

        dialog->setStatus("connectting server ip:"+ip+"  port:" + QString::number(port));
}

void ClientNetworkControl::DialogCancelHandle(ConnectDialog *dialog)
{

    dialog->getEnterButton()->setEnabled(true);
    dialog->getCancelButton()->setEnabled(false);

    dialog ->getIpLineEdit()->setEnabled(true);
    dialog->getPortLineEdit()->setEnabled(true);
    dialog->getNameLineEdit()->setEnabled(true);
    dialog->getPasswdLineEdit()->setEnabled(true);

     clientmodel ->close();
     dialog->setStatus(QString("no start. \nnotice:port must be integer,passwd must be integer or letter."));

}

void ClientNetworkControl::DialogExitHandle(ConnectDialog *dialog)
{
    /*close*/
     if(clientmodel !=nullptr )
     {
         delete clientmodel ;
         clientmodel =nullptr;
     }
     if(dialog!=nullptr)
     {
         dialog->close();
     }
}

void ClientNetworkControl::initDialog(ConnectDialog *dialog)
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
    ipline->setEnabled(true);
    dialog->setRegExpValidator("^((25[0-5]|2[0-4]\\d|[01]?\\d\\d?)\\.){3}(25[0-5]|2[0-4]\\d|[01]?\\d\\d?)$",dialog->getIpLineEdit());
    dialog->setRegExpValidator("^\\d+$",dialog->getPortLineEdit());//非负整数
    dialog->setRegExpValidator("^[A-Za-z0-9]+$",dialog->getPasswdLineEdit());//字母或数字
    dialog->setStatus(QString("no start. \nnotice:port must be integer,passwd must be integer or letter."));
    dialog->getEnterButton()->setEnabled(true);
    dialog->getCancelButton()->setDisabled(true);
}

void ClientNetworkControl::initGame(QString selfname,QString remotename,int timeout )
{
    /*设置棋手完毕*/
        blacker = new ChesserMan(selfname.toStdString(),SERVER_COLOR);
        whiteer = new ChesserMan(remotename.toStdString(),CLIENT_COLOR);
        boardmodel = new BoardModel(blacker,whiteer);

        boardframe = new BoardFrame(boardmodel,this,550,boardmodel->getBoardSize());
        showinfo_ui = new ShowInfoFrameUi(this,boardmodel,QString(blacker->getName().c_str()),QString(whiteer->getName().c_str()),timeout);
        chatframe = new ChatFrameui;
        /*初始化model和view完毕*/
        boardmodel->addObserver(boardframe);
        boardmodel->addObserver(showinfo_ui);
        /*将观察者 注册 到 主题完毕*/
        frame = new QFrame();
        frame->setFixedSize(850,550);
        chatframe->setFixedSize(showinfo_ui->width(),showinfo_ui->height()/3 );
        chatframe->setParent(showinfo_ui);
        boardframe->setParent(frame);
        showinfo_ui->setParent(frame);
        boardframe->move(0,0);
        showinfo_ui->move(550,0);
        chatframe->move(0,showinfo_ui->height()-chatframe->height());

        /*UI窗口的组合完毕*/
        connect(chatframe->getEnterButton(),&QAbstractButton::clicked,this,&ClientNetworkControl::sendMessagehandle);
        end_flag = END_FLAGS::RUN;
        /*初始化完毕*/

        frame->show();
        /*显示窗体*/

}

void ClientNetworkControl::exitHandle()
{
    frame->close();
    exit(0);
}

void ClientNetworkControl::gameOverHandle(ChessColorPro who)
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
