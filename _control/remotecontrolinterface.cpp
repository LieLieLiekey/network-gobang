#include "remotecontrolinterface.h"
#include <QMessageBox>
void RemoteControlInterface::errorHanle(EXCEPT_LEVEL level,QString errormsg)
{
    if(level==EXCEPT_LEVEL::NO)
        return ;
    QMessageBox msg;
    switch(level)
    {
    case EXCEPT_LEVEL::LOW:
        msg.setText("EXCEPT_LEVEL::LOW.info:" + errormsg);
        msg.exec();
        break;
    case EXCEPT_LEVEL::MID:
        msg.setText("EXCEPT_LEVEL::MID.info:"+ errormsg);
        msg.exec();
        break;
    case EXCEPT_LEVEL::HIGH:
        msg.setText("EXCEPT_LEVEL::HIGH.info:."+ errormsg);
        msg.exec();
        break;
    case EXCEPT_LEVEL::SERIOUS:
        msg.setText("EXCEPT_LEVEL::SERIOUS.info:"+ errormsg);
        msg.exec();
        exit(1);
        break;
    default:
        break;
        //no able
    }
}
