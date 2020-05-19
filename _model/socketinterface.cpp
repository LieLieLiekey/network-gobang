#include "socketinterface.h"

SocketInterface::SocketInterface()
{

}

void SocketInterface::sendMessage(QString info)
{
    int len=0;
    send_buf[len++]=MESSAGE_FLAGS::MESSAGE;
    strcpy_s(send_buf + len,sendbuf_size-len,info.toStdString().c_str());
    len +=info.toStdString().length();
    writeData(send_buf,len);

}
