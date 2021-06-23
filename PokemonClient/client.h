#ifndef CLIENT_H
#define CLIENT_H
#include<QString>
#include<QTcpSocket>
#include<QDebug>
#include<QWidget>
#include<QObject>
#include<QJsonDocument>
#include<QJsonObject>
class Client:public QWidget
{
    Q_OBJECT
public:
    explicit Client(QWidget *parent = 0);

    ~Client();
    void setUserName(QString name){userName=name;}//设置用户名
    QString getUserName()const//得到用户名
    {return userName;}
    bool connected() const//判断客户端是否连接上服务器
    {return isConnected;}

    void socketConnect(char *addr);//socket连接
    void socketDisconnect();//socket断开连接
    QJsonObject sendMsg(QJsonObject requestInfo);//发送数据给服务器，返回值为服务器回复

private:
    QString userName;//用户名
    QTcpSocket *socket;//socket
    bool isConnected;//是否连接到服务器
    int port=7359;//服务器端口
    char serverAddr[16]="127.0.0.1";//服务器IP地址
    QByteArray recvBuff;//接收缓冲区

};

#endif // CLIENT_H
