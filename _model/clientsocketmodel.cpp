
#include "clientsocketmodel.h"
#include "remotecontrolinterface.h"
#include <QString>
/*
区别：一个客户端，一个服务器
服务器先发送PW，再接受PWOK
    static constexpr int BUFSIZE = 256;
    static constexpr int sendbuf_size=16;
    char send_buf [sendbuf_size];
    QString _selfname,_remote_name,_passwd,_ip;
    int _port;
    RemoteControlInterface * _remote_control;
    QTcpSocket *_client;
    using STATE = enum{S_NO,S_READY,S_ACCPET,S_RECVPW,S_SENDPW,S_START,S_END};
    STATE _state;

*/
ClientSocketModel::ClientSocketModel(RemoteControlInterface * remote_control)
{
    _remote_control = remote_control;
    _selfname = "";
    _remote_name ="";
    _port = -1;
    _passwd ="";
    _ip="localhost";
    _client = new QTcpSocket;
    _state = STATE::NO;
    connect(_client,&QAbstractSocket::connected,[&]{
      connectHandle(_client);
    });
    connect(_client,&QAbstractSocket::readyRead,[&]{
       readHanele(_client);
    });
    connect(_client,&QAbstractSocket::disconnected,[&]{
       disConnectHandle(_client);
    });
}
ClientSocketModel::~ClientSocketModel()
{
    if(_client !=nullptr)
    {
        _client->close();
        delete  _client;
    }

}
void ClientSocketModel::sendPOS(int x,int y)
{
    send_buf[0]=MESSAGE_FLAGS::POS;
    send_buf[1]=x;
    send_buf[2]=y;
    writeData(send_buf,3);
}
void ClientSocketModel::sendEXIT()
{
    send_buf[0]=MESSAGE_FLAGS::EXIT;
    writeData(send_buf,1);
}
void ClientSocketModel::sendGIVEUP()
{
    send_buf[0]=MESSAGE_FLAGS::GIVEUP;
    writeData(send_buf,1);
}

void ClientSocketModel::sendPW()//发送pw和自己的名字
{
    /*[flag][PW]['\0'][name]*/
    int len =0;
    send_buf[len++]=MESSAGE_FLAGS::PW;

    strcpy_s(send_buf+len,sendbuf_size - len,_passwd.toStdString().c_str());
    len +=_passwd.toStdString().length();

    send_buf[len++]='\0';

    strcpy_s(send_buf+len,sendbuf_size - len,_selfname.toStdString().c_str());
    len+=_selfname.toStdString().length();

    writeData(send_buf,len);
    _state =STATE::SENDPW;
}
void ClientSocketModel::writeData(char buf[],int len)
{
    _client->write(buf,len);
    _client->flush();
}

void ClientSocketModel::recvPOS(char buf[],int len)
{
    buf++,len--;
    int x = buf[0],y = buf[1];
    _remote_control ->remotePutChessSignal(Position(x,y));
}
void ClientSocketModel::recvEXIT(char buf[],int len)
{
     _remote_control ->remoteExitSignal();
}
void ClientSocketModel::recvGIVEUP(char buf[],int len)
{
_remote_control ->remoteGiveUpSignal();
}
void ClientSocketModel::recvPWOK(char buf[],int len)
{
    buf++,len--;//去除消息体
    _remote_name = "";
    for(int i=0;i<len;++i) _remote_name.append(buf[i]);
    _remote_control ->remoteBeginGameSignal();
    _state = STATE::START;
}
void ClientSocketModel::readHanele(QTcpSocket *client)
{
    static char buf[BUFSIZE];
    static int tol = 0,LEN_SIZE=2;
    int len =_client -> read(buf+tol,BUFSIZE - tol);
    if(len  == -1){
        /*a error*/
        return ;
    }
    tol +=len;
    if(tol >=LEN_SIZE ){
        int datalen = (buf[0]<<8) +buf[1];//读取大端序的前2个字节
        if(datalen + LEN_SIZE < tol){
            /*不能读*/
            return ;
        }
        switch (buf[LEN_SIZE])
        {
        case MESSAGE_FLAGS::PWOK:
                recvPWOK(buf + LEN_SIZE,datalen - LEN_SIZE);
            break;
        case MESSAGE_FLAGS::END:
                 /*no sign*/
            break;
        case MESSAGE_FLAGS::POS:
            recvPOS(buf + LEN_SIZE,datalen - LEN_SIZE);
            break;
        case MESSAGE_FLAGS::EXIT:
            recvEXIT(buf + LEN_SIZE,datalen - LEN_SIZE);
            break;
        case MESSAGE_FLAGS::GIVEUP:
            recvGIVEUP(buf + LEN_SIZE,datalen - LEN_SIZE);
            break;
        case MESSAGE_FLAGS::TIMEOUT:
            /*no operator*/
            break;
        default:
            break;
            // a error
        }
        for(int i=0 ,j =datalen; j < tol ;++i,++j)
            buf[i]=buf[j];
        tol -=datalen;

    }
}
void ClientSocketModel::close()
{
    _state = STATE::END;
    if(_client!=nullptr)
    {
        _client->disconnectFromHost();
        _client->close();
    }
}
void ClientSocketModel::disConnectHandle(QTcpSocket *client)
{
    _client->close();
    _remote_control ->remoteDisConnectSignal();
    _state = STATE::NO;
}
void ClientSocketModel::connectHandle(QTcpSocket *client)
{
   _remote_control->remoteBeginGameSignal();
   _state = STATE::START;
}
bool ClientSocketModel::start(QString name,QString passwd,int port,QString ip)
{
    _selfname = name;
    _passwd = passwd;
    _port = port;
    close();
    _client->connectToHost(ip,port);
    _state = STATE::READY;
    //_ip = getServerAddress();//getNowIp();
    return true;
}
QString ClientSocketModel::getServerAddress()
{
    return "0.0.0.0";
}
int ClientSocketModel::getServerPort()
{
    return 0;
}

QString ClientSocketModel::getRemoteName()
{
    return _remote_name;
}
bool ClientSocketModel::alreadyBegin()
{
    return _state == STATE::START;
}

/*

    static constexpr int BUFSIZE = 256;
    static constexpr int sendbuf_size=16;
     char send_buf [sendbuf_size];
     static constexpr int BUFSIZE = 256;
     static constexpr int sendbuf_size=16;
      char send_buf [sendbuf_size];
   QString _selfname,_remote_name,_passwd,_ip;
   int _port;
   RemoteControlInterface * _remote_control;
   QTcpSocket *_client;
   using STATE = enum{S_NO,S_READY,S_ACCPET,S_RECVPW,S_SENDPW,S_START,S_END};
   STATE _state;
*/
