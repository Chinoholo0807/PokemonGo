#ifndef LOGWidget_H
#define LOGWidget_H

#include <QWidget>
#include"client.h"
#include"stackwidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class LogWidget : public QWidget
{
    Q_OBJECT

public:
    LogWidget(QWidget *parent = nullptr);
    ~LogWidget();

    void slotPushButtonLogin();//按下注册按钮
    void slotPushButtonRegister();//按下注册按钮
private:
    Ui::Widget *ui;//ui界面指针
    Client * client;//客户端socket
};
#endif // LogWidget_H
