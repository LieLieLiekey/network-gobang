#include "serversocketmodel.h"
#include <QHostAddress>
#include <QList>
#include <QNetworkInterface>
/*
不定式：
1._server 永远不为空
2.只有当连接一个client时，client才为true
- 收到远程信息会及时通知对面。
2.没有listen时，close时state为No,
3.当listen时，state 为READY
3.当accept时候，state为ACCEPT
4.PW正确时，state为RECVPW
- state 为 BEGIN
- 只能连接一个客户端
-
*/
void ServerSocketModel::recvMessage(char buf[],int len)
{
    len--,buf++;
    QString msg;
    for(int i=0;i<len;++i) msg.append(buf[i]);
    _remote_control ->remoteMessageSignal(msg);
}
ServerSocketModel::ServerSocketModel(RemoteControlInterface *remote_control)
{
    _remote_control = remote_control;
    _selfname = "";
    _remote_name ="";
    _port = -1;
    _passwd ="";
    _ip="localhost";
    _client =nullptr;
    _server = new QTcpServer;
    _state = STATE::NO;
    connect(_server, &QTcpServer::newConnection, this, &ServerSocketModel::acceptHandle);
}
ServerSocketModel::~ServerSocketModel()
{
   if(_client !=nullptr)
   {
       _client->close();
   }
   if(_server !=nullptr)
   {
       _server->close();
       delete  _server;
   }
}
QString ServerSocketModel::getServerAddress()
{
    if(_server == nullptr) return "";
//    else return _server->serverAddress().toString();
     QString ipAddress;
     QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
     // use the first non-localhost IPv4 address
     for (int i = 0; i < ipAddressesList.size(); ++i) {
         if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
             ipAddressesList.at(i).toIPv4Address()) {
             ipAddress = ipAddressesList.at(i).toString();
             break;
         }
     }
     // if we did not find one, use IPv4 localhost
     if (ipAddress.isEmpty())
         ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
     return ipAddress;
}
int ServerSocketModel::getServerPort()
{
    if(_server == nullptr) return -1;
    else return _server->serverPort();
}


bool ServerSocketModel::start(QString name,QString passwd,int port,QString ip)
/*
   配置服务器，并监听
*/
{
    _selfname = name;
    _passwd = passwd;
    _port = port;
    close();
    if(!_server->listen(QHostAddress::Any,_port))
    {
        return false;
    }
    _state = STATE::READY;
    //_ip = getServerAddress();//getNowIp();
    return true;

}
void ServerSocketModel::readHanele(QTcpSocket *client)
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
        case MESSAGE_FLAGS::PW:
                recvPW(buf + LEN_SIZE,datalen);
            break;
        case MESSAGE_FLAGS::END:
                 /*no sign*/
            break;
        case MESSAGE_FLAGS::POS:
            recvPOS(buf + LEN_SIZE,datalen);
            break;
        case MESSAGE_FLAGS::EXIT:
            recvEXIT(buf + LEN_SIZE,datalen);
            break;
        case MESSAGE_FLAGS::GIVEUP:
            recvGIVEUP(buf + LEN_SIZE,datalen);
            break;
        case MESSAGE_FLAGS::TIMEOUT:
            /*no operator*/
            break;
        case MESSAGE_FLAGS::MESSAGE:
            recvMessage(buf + LEN_SIZE,datalen);
            break;
        default:
            //qDebug() << "no sign!!!!!!1\n";
            break;
            // a error
        }
        for(int i=0 ,j =datalen; j < tol ;++i,++j)
            buf[i]=buf[j];
        tol -=datalen+LEN_SIZE;

    }
}
void ServerSocketModel::close()
/*
    关闭客户端和服务器套接字，重置状态
*/
{
    _state = STATE::END;
    if(_client!=nullptr)
        _client->close();
    if(_server!=nullptr)
        _server->close();
    _client = nullptr;
}
void ServerSocketModel::acceptHandle()
{
    /*只允许有一个连接*/
    if(_client ==nullptr)
    {
        QTcpSocket *clientConnection = _server->nextPendingConnection();
        connect(clientConnection, &QAbstractSocket::disconnected,[&]
        {
            disConnectHandle(clientConnection);
        });
        //连接读请求
        connect(clientConnection,&QAbstractSocket::readyRead,[&](){
            readHanele(clientConnection);
        });

        _client = clientConnection;
        _state = STATE ::ACCPET;
    }
}
void ServerSocketModel::disConnectHandle(QTcpSocket *client)
{
    /*
    被动断开连接：关闭套接字，改变状态，
    */
       if(_state == STATE::START){
           _remote_control ->remoteDisConnectSignal();
       }
       _state =STATE::READY;
       _client->close();
       _client = nullptr;
}
void ServerSocketModel::writeData(char buf[],int len)
{
    static QByteArray writebytebuf;
    writebytebuf.clear();
    writebytebuf.append(char(len>>8));
    writebytebuf.append(char(len&(0xff)));
    writebytebuf.append(buf,len);
    _client->write(writebytebuf);
    _client->flush();
//    qDebug()<<"\n writeDate \n";
//    for(int i=0;i<len;++i){
//        qDebug() << buf[i];
//    }
//    qDebug()<<"\n";
//    _client->write(buf,len);
//    _client->flush();
}

void ServerSocketModel::recvPOS(char buf[],int len)
{
    /* info:[flag,1][pos1,1][pos2,1]*/
    buf++,len--;
    int x = buf[0],y = buf[1];
    _remote_control ->remotePutChessSignal(Position(x,y));
}
void ServerSocketModel::recvEXIT(char buf[],int len)
{
    _remote_control ->remoteExitSignal();
}
void ServerSocketModel::recvGIVEUP(char buf[],int len)
{
    _remote_control ->remoteGiveUpSignal();
}
static bool str_equal(const char *s1,int len1 ,const char *s2,int len2)
{
    if(len1 != len2) return false;
    for(int i = 0 ;i < len1 ;++i){
        if(s1[i]!=s2[i]) return false;
    }
    return true;
}
void ServerSocketModel::recvPW(char buf[],int len)
{
    /* info:[flag,1][passwd]['\0'][remote_name]*/
    buf++,len--;//去除消息体
    int p=0;
    while(p < len && buf[p]!='\0') ++p;
    if( p > len)
    {
        // no find '\0', a error
        return ;
    }
    else{// [buf - p ),[p,buf+len)
        if( str_equal( buf,p,_passwd.toStdString().c_str(),int(_passwd.toStdString().length()) ) )
        {
            _state = STATE::RECVPW;
            for(int i=p+1 ;i < len ;++i)
                _remote_name.append(buf[i]);
            _remote_control->remotePasswdCurrect();
            sendPWOK();
            return ;
        }
        else{
            //error
            return ;
        }

    }

}

void ServerSocketModel::sendPOS(int x,int y)
{
    send_buf[0]=MESSAGE_FLAGS::POS;
    send_buf[1]=x;
    send_buf[2]=y;
    writeData(send_buf,3);
}
void ServerSocketModel::sendEXIT()
{
    send_buf[0]=MESSAGE_FLAGS::EXIT;
    writeData(send_buf,1);
}
void ServerSocketModel::sendGIVEUP()
{
    send_buf[0]=MESSAGE_FLAGS::GIVEUP;
    writeData(send_buf,1);
}
void ServerSocketModel::sendPWOK()//发送pw和自己的名字
{
    /*PWOK:[flag,1][name]*/
    send_buf[0]=MESSAGE_FLAGS::PWOK;
    int len =1+_selfname.toStdString().length();;
    strcpy_s(send_buf+1,sendbuf_size -1,_selfname.toStdString().c_str());
    writeData(send_buf,len);
    _state =STATE::START;
    _remote_control ->remoteBeginGameSignal();

}
QString ServerSocketModel::getRemoteName()
{
   return _remote_name;
}
bool ServerSocketModel::alreadyBegin()
{
    return _state == STATE::START;
}
