#ifndef CHATFRAMEUI_H
#define CHATFRAMEUI_H

#include <QWidget>

namespace Ui {
class ChatFrameui;
}
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
class ChatFrameui : public QWidget
{
    Q_OBJECT

public:
    explicit ChatFrameui(QWidget *parent = nullptr);
    ~ChatFrameui();
    QPushButton* getEnterButton();
    QString getLineInfo();
    QString getTextInfo();
    void appendLine(QString info);
    void appendMessage(QString name,QString info);
    void clearlineInfo();

private:
    static constexpr int MAX_TEXTLEN=4096;
    Ui::ChatFrameui *ui;
    QTextEdit *text_edit;
    QLineEdit *line_edit;
    QPushButton *send_bt;
};

#endif // CHATFRAMEUI_H
