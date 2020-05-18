QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
msvc:QMAKE_CXXFLAGS += -execution-charset:utf-8
msvc:QMAKE_CXXFLAGS += -source-charset:utf-8
QMAKE_CXXFLAGS_WARN_ON += -wd4819
SOURCES += \ \
    _control/clientnetworkcontrol.cpp \
    _control/controlinterface.cpp \
    _control/localcontrol.cpp \
    _control/networkcontrol.cpp \
    _control/remotecontrolinterface.cpp \
    _control/servernetworkcontrol.cpp \
    _model/boardmodel.cpp \
    _model/boardmodelinterface.cpp \
    _model/chesser.cpp \
    _model/chesserai.cpp \
    _model/chesserman.cpp \
    _model/clientsocketmodel.cpp \
    _model/observable.cpp \
    _model/observer.cpp \
    _model/position.cpp \
    _model/serversocketmodel.cpp \
    _model/socketinterface.cpp \
    _view/boardframe.cpp \
    _view/connectdialog.cpp \
    _view/selectmodeldialogui.cpp \
    _view/showinfoframeui.cpp \
    main.cpp

HEADERS += \ \
    _control/clientnetworkcontrol.h \
    _control/controlinterface.h \
    _control/localcontrol.h \
    _control/networkcontrol.h \
    _control/remotecontrolinterface.h \
    _control/servernetworkcontrol.h \
    _model/ChessColorPro.h \
    _model/boardmodel.h \
    _model/boardmodelinterface.h \
    _model/chesser.h \
    _model/chesserai.h \
    _model/chesserman.h \
    _model/clientsocketmodel.h \
    _model/observable.h \
    _model/observer.h \
    _model/position.h \
    _model/serversocketmodel.h \
    _model/socketinterface.h \
    _view/boardframe.h \
    _view/connectdialog.h \
    _view/selectmodeldialogui.h \
    _view/showinfoframeui.h

FORMS += \ \
    _view/connectdialog.ui \
    _view/selectmodeldialogui.ui \
    _view/showinfoframeui.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

DISTFILES +=

INCLUDEPATH += \ \
_control/ \
_model/ \
_view/ \
