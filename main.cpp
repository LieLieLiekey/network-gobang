
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
#include "chatframeui.h"
int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   ChatFrameui dch;
   dch.appendLine("dasdasdas");
   dch.appendLine("dasdasdas");
   dch.appendLine("dasdasdas");
   dch.show();

   /*
   ServerNetworkControl dch;
   dch.start()*/;

   a.exec();
   return 0;
}
