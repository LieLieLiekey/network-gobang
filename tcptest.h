#ifndef TCPTEST_H
#define TCPTEST_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
namespace Ui {
class TcpTest;
}

class TcpTest : public QWidget
{
    Q_OBJECT

public:
    explicit TcpTest(QWidget *parent = nullptr);
    ~TcpTest();
    QTextEdit  *textView;
    QPushButton * exitbt;
private:
    Ui::TcpTest *ui;
};

#endif // TCPTEST_H
