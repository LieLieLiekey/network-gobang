#include "socketinterface.h"

SocketInterface::SocketInterface()
{

}

void SocketInterface::sendMessage(QString info)
{
    int len=0;
    send_buf[len++]=MESSAGE_FLAGS::MESSAGE;
    strcpy_s(send_buf + len,sendbuf_size-len,info.toUtf8().data());
    len +=info.toUtf8().length();
    writeData(send_buf,len);

}
