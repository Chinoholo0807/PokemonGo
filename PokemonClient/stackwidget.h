#ifndef STACKWIDGET_H
#define STACKWIDGET_H

#include <QWidget>
#include<QStackedWidget>
#include<QMessageBox>
#include"hallwidget.h"
#include"fightwidget.h"
#include"fightingwidget.h"
#include"packwidget.h"
#include"client.h"
#include"userlistwidget.h"
#include"userinfo.h"
#include"define.h"
#include"pokemon.h"
namespace Ui {
class StackWidget;
}
//管理多个界面之间的切换
class StackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StackWidget(Client *userClient,QWidget *parent = nullptr);//构造函数
    ~StackWidget();//析构函数

    QJsonObject sendRequestToServer(QJsonObject&);//向服务器发送请求,返回回复信息
    void setCurrentIndex(int idx);//设置当前界面
    void BackToHall();//返回大厅
    bool trySynchronous();//尝试服务器同步信息
public slots:
    void tryEnterPackWidget();//申请进入用户背包界面
    void tryEnterFightWidget();//申请进入战斗大厅
    void tryCheckOtherUser(QString userName);//申请查看其他用户精灵
    void tryEnterFightingWidget(int mode,Pokemon*A,Pokemon*B,int autoFlag);//申请进入战斗场景
    void tryEnterUserListWidget();//申请进入在线用户场所
    //申请从战斗场所返回战斗大厅,处理战斗结果
    void tryBackToHallFromFightingWidget(Pokemon*pokemonA,Pokemon*pokemonB,int gameMode,int packIdx);
    //申请返回大厅
    void tryBackToHall();
private:
    void initWidget();//初始化窗口，包括UI和槽函数
private:
    Client * client;//当前用户
    UserInfo userInfo;//用户信息
    QList<Pokemon*>pokemonPack;//利用用户信息中的pack实例化的小精灵列表
    Ui::StackWidget *ui;//ui界面
    QStackedWidget* stackedWidget;//stackedWidget用来切换界面
    HallWidget* hallWidget;//游戏大厅界面
    FightWidget* fightWidget;//战斗大厅界面
    FightingWidget* fightingWidget;//战斗场景界面
    PackWidget * packWidget;//用户背包界面
    UserListWidget * userListWidget;//用户列表界面

};

#endif // STACKWIDGET_H
