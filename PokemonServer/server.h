#ifndef SERVER_H
#define SERVER_H
#include<QTcpSocket>
#include<QTcpServer>
#include<QWidget>
#include<QSqlDatabase>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT


public:
    explicit Server(QWidget *parent = nullptr);
    void startServer();//开启服务器
    void stopServer();//关闭服务器
    void sendMsgToClients(QJsonObject responseInfo,QTcpSocket*client);//发送responseInfo给client
    void initDatabase();//初始化数据库
    QList<QString> getOnlineUser()const{return onlineUser;}//获取在线用户列表
    QList<QString> getOtherUser()const;//获取离线用户列表
    QTcpSocket* getOnlineUserSocket(int idx)const{return onlineUserSocket.at(idx);}//获取某一用户的socket以便通信
    bool isOnline(QString userName)const;//判断用户名为userName的用户是否在线
    void addOnlineUser(QString userName,QTcpSocket*socket);//将在线用户添加到在线用户列表
    QSqlDatabase getDatabase()const{return db;}//获取db
    void paintEvent(QPaintEvent *event);
    void deleteOfflineUser(QString userName,QTcpSocket*socket);//将离线用户从在线列表中删除,利用userName或socket
public slots:

    void newClientConnection();//有新的客户端加入连接
    void socketDisconnected();
    void socketReadyRead();
    void socketStateChanged(QAbstractSocket::SocketState state);

protected:
    Ui::Widget *ui;
    QTcpServer*             chatServer;//TCP服务器
    QVector<QTcpSocket*>*   allClients;//存储client的socket
    const int port=7359;//监听端口
    QSqlDatabase db;//数据库
    QList<QString> onlineUser;//在线用户列表
    QList<QTcpSocket*> onlineUserSocket;//在线用户对应的socket

};
#endif // WIDGET_H
