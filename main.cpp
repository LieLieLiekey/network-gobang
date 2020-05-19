
#include <QApplication>
#include "boardframe.h"
#include <QFrame>
#include "boardmodel.h"
#include "localcontrol.h"
#include "showinfoframeui.h"
#include "selectmodeldialogui.h"
#include "QTcpSocket"
#include "connectdialog.h"
#include "servernetworkcontrol.h"
#include <QDebug>
#include <QMessageBox>
#include <clientnetworkcontrol.h>
#include "maindialog.h"
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   LocalControl *local=nullptr;
   ServerNetworkControl *server=nullptr;
   ClientNetworkControl *client=nullptr;
    mainDialog  sel;
    sel.exec();
    if(sel.isEnter())
    {
        switch (sel.getModel())
        {

        case mainDialog::MODEL::LOCAL:
                local = new LocalControl;
                local->start();
            break;

        case mainDialog::MODEL::SERVER:
            server = new ServerNetworkControl;
            server->start();
            break;
        case mainDialog::MODEL::CLIENT:
            client = new ClientNetworkControl;
            client->start();
            break;
        default:
            // no can reach there
            break;
        }
    }
    else{
        exit(0);
    }
//   ServerNetworkControl dch;
//   dch.start();
   a.exec();
   if(local !=nullptr) delete local;
   if(server !=nullptr) delete server;
   if(client !=nullptr) delete client;
   return 0;
}
