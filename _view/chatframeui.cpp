#include "chatframeui.h"
#include "ui_chatframeui.h"

ChatFrameui::ChatFrameui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatFrameui)
{
    ui->setupUi(this);
    text_edit  = ui->text_edit;
    line_edit =  ui->line_edit;
    send_bt = ui->send_button;
}

ChatFrameui::~ChatFrameui()
{
    delete ui;
}
QPushButton *ChatFrameui::getEnterButton()
{
    return send_bt;
}
void ChatFrameui::appendLine(QString info)
{
    if(text_edit->toPlainText().length()  >= MAX_TEXTLEN){
        text_edit->clear();
        text_edit->append("last message already clear.");
    }
    text_edit->append(info);
}
void ChatFrameui::appendMessage(QString name,QString info)
{
   this->appendLine(name+":"+info);
}
QString ChatFrameui::getLineInfo()
{
    return line_edit->text();
}
QString ChatFrameui::getTextInfo()
{
    return text_edit->toPlainText();
}
void ChatFrameui::clearlineInfo()
{
    line_edit->clear();
}
