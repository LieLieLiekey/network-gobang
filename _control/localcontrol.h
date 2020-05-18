#ifndef CONTROL_H
#define CONTROL_H

#include "boardmodel.h"
#include "boardframe.h"
#include "chesser.h"
#include "boardmodelinterface.h"
#include "chesserman.h"
#include "chesserai.h"
#include "controlinterface.h"
#include "showinfoframeui.h"
class ShowInfoFrameUi;
class BoardFrame;
class Chesser;
class BoardModelInterface;
class ControlInterface;
class LocalControl:public ControlInterface
{
//    Q_OBJECT
public:
    LocalControl();//s
    ~LocalControl();
    void putChessSignal(Position pos) override;
    void giveUpSignal(ChessColorPro ) override;
    void exitSignal(ChessColorPro) override;
    void timeOutSingal(ChessColorPro) override;
    void start(int timeout = 600);
private :
    void gameOverHandle();
    void setAIModel(QString black_name);
    void exitHandle();
    void setDoubleModel(QString black_name,QString white_name);
    BoardModel * boardmodel;
    Chesser *whiteer,*blacker;
    BoardFrame * boardframe;
    ShowInfoFrameUi *showinfo_ui;
    QFrame *frame;
    using GAME_MODEL=enum{AI_MODEL,DOUBLE_MODEL,DOG_MODEL};
    using END_FLAGS=enum{TIMEOUT,GAVEUP,END,NOEND};
    END_FLAGS end_flag;
    GAME_MODEL game_model;
};

#endif // CONTROL_H
