#ifndef USERLISTWIDGET_H
#define USERLISTWIDGET_H

#include <QWidget>

namespace Ui {
class UserListWidget;
}

class UserListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserListWidget(QWidget *parent = nullptr);//构造函数
    ~UserListWidget();//析构函数
    void setOnlineUser(QStringList userList){onlineUser=userList;}//设置在线用户
    void setOtherUser(QStringList userList){otherUser=userList;}//设置离线用户
    void reflesh();//刷新窗口
    void paintEvent(QPaintEvent *event) override;//重写paintEvent
signals:
    void back();//返回大厅信号
    void checkPokemon(QString userName);//查看其他用户精灵
private:
    Ui::UserListWidget *ui;//ui界面指针
    int idx;
    QStringList onlineUser;//在线用户列表
    QStringList otherUser;//离线用户列表
};

#endif // USERLISTWIDGET_H
