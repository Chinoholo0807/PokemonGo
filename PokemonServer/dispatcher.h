#ifndef DISPATCHER_H
#define DISPATCHER_H
#include<QTcpSocket>
#include<QJsonObject>
#include<QJsonDocument>
#include<QSqlDatabase>
#include<QSqlQuery>
#include"server.h"
#include"userinfo.h"
//用于任务调度的类
class Dispatcher
{
public:
    Dispatcher();
    ~Dispatcher()=default;
    Dispatcher(QTcpSocket *socket,Server *parent =nullptr,QString userName=""):\
        socket(socket),parent(parent),userName(userName){

    }

    QJsonObject dispatch(QJsonObject requestInfo);//dispatch任务调度
private:
    QJsonObject loginHandle(QJsonObject&);//登录处理逻辑
    QJsonObject registerHandle(QJsonObject&);//注册处理逻辑
    QJsonObject userInfoHandle(QJsonObject&);//用户信息申请处理逻辑
    QJsonObject onlineUserHandle(QJsonObject&);//查看在线用户处理逻辑
    QJsonObject game0ResultHandle(QJsonObject&);//同步用户升级赛战斗数据处理逻辑
    QJsonObject game1ResultHandle(QJsonObject&);//同步用户决斗赛战斗数据处理逻辑
private:
   QTcpSocket * socket;//与dispatcher绑定的socket
   Server *parent;//调用dispatcher的对象,用于访问server内容
   QString userName;//socket相对于的userName

};

#endif // DISPATCHER_H
