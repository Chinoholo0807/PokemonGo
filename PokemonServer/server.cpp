
#include "server.h"
#include"ui_server.h"
#include"dispatcher.h"
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QPainter>
using namespace std;
Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    setWindowTitle("服务器端");
    setWindowIcon(QIcon(":/pic/icon.ico"));
    setFixedSize(300,400);
    startServer();
}
//启动server
void Server::startServer()
{
    //启动服务器

    //初始化数据库
    initDatabase();

    //初始化服务器
    allClients = new QVector<QTcpSocket*>;
    chatServer = new QTcpServer();
    //设置最大连接数
    chatServer->setMaxPendingConnections(10);
    //连接槽函数
    connect(chatServer,&QTcpServer::newConnection, this, &Server::newClientConnection);
    if (chatServer->listen(QHostAddress::Any, port))
        qDebug() << QString("Server has started. Listening to port %1.").arg(port);
    else{
        qDebug() << "Server failed to start. Error: " + chatServer->errorString();
        exit(1);
    }


}
//初始化数据库
void Server::initDatabase(){
    db=QSqlDatabase::addDatabase("QSQLITE");
    //如果本目录下没有该文件,则会在本目录下生成,否则连接该文件
    QString dbName=QApplication::applicationDirPath()+"/userdata.dat";
    db.setDatabaseName(dbName);
    if (!db.open()) {
         qDebug()<<"Open database "+dbName+" failed.";
         exit(1);
     }
    qDebug()<<"Open database "+dbName+" succeeded.";
    QSqlQuery query(db);
    //检查表项是否已经存在
    QStringList strTables = db.tables();
    if(!strTables.contains("user")){//表项不存在
        //创建新表
        qDebug() << "Table user does not exist";
        query.exec("drop table user");
        query.exec("create table user(user_name varchar primary key,\
                   password varchar,win_count int,lost_count int,pokemon_count int)");
         //测试账号
        query.exec("insert into user values('test1','1111',10,15,10)");
        query.exec("insert into user values('test2','2222',5,5,1)");
        query.exec("drop table pack");
        query.exec("create table pack(id int  primary key,user_name varchar,\
                   uid int ,level int ,exp int)");
        query.exec("insert into pack values(0,'test1',0,15,0)");
        query.exec("insert into pack values(1,'test1',1,15,0)");
        query.exec("insert into pack values(2,'test1',2,15,0)");
        query.exec("insert into pack values(3,'test1',3,15,0)");
        query.exec("insert into pack values(4,'test1',4,15,0)");
        query.exec("insert into pack values(5,'test1',0,10,1)");
        query.exec("insert into pack values(6,'test1',1,10,1)");
        query.exec("insert into pack values(7,'test1',2,10,1)");
        query.exec("insert into pack values(8,'test1',3,10,1)");
        query.exec("insert into pack values(9,'test1',4,10,1)");
        query.exec("insert into pack values(10,'test2',2,1,1)");
        query.exec("select * from user");
    }else
        qDebug() << "Table user exist";
    query.exec("delete from pack where id == 5724");
    query.exec("select * from user");
    while(query.next()){
        qDebug()<<"userName: "<<query.value(0).toString()<<"  password: "<<query.value(1).toString()\
               <<" winCount: "<<query.value(2).toInt()<<" lostCount: "<<query.value(3).toInt()\
              <<" pCount: "<<query.value(4).toInt();
    }
    query.exec("select * from pack");
    while(query.next()){
        qDebug()<<" id: "<<query.value(0).toInt()<<" ownerName: "<<query.value(1).toString()\
             <<" uid: "<<query.value(2).toInt()<<" level: "<<query.value(3).toInt()\
            <<" exp: "<<query.value(4).toInt();
    }


}
//发送msg给client
void Server::sendMsgToClients(QJsonObject responseInfo,QTcpSocket *client)
{
    QString tmp=QString("Send responseInfo to [%1:%2].")\
            .arg(client->peerAddress().toString()).arg(client->peerPort());
    qDebug()<<tmp;

    QJsonDocument doc(responseInfo);
    client->write(doc.toJson());
    qDebug()<<doc.toJson();
}
//有新的client加入connect
void Server::newClientConnection()
{
    QTcpSocket* client = chatServer->nextPendingConnection();
    QString ipAddress = client->peerAddress().toString();
    int port = client->peerPort();
    connect(client, &QTcpSocket::disconnected, this, &Server::socketDisconnected);
    connect(client, &QTcpSocket::readyRead, this, &Server::socketReadyRead);
    connect(client, &QTcpSocket::stateChanged, this, &Server::socketStateChanged);
    allClients->push_back(client);
    qDebug() << "Socket connected from [" + ipAddress + ":" + QString::number(port)+"]";
}
//有client断开连接
void Server::socketDisconnected()
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();
    qDebug() << "Socket disconnected from [" + socketIpAddress + ":" + QString::number(port)+"]";\
    //从socket列表中移除
    for(int i=0;i<allClients->size();i++){
        if(client == allClients->at(i)){
            allClients->remove(i);
            break;
        }
    }
    //若存在于在线列表中，从在线列表中删除
    deleteOfflineUser("unknown",client);
}
//有client发送数据给server
void Server::socketReadyRead()//server收到msg
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();
    //接收msg
    QByteArray recvBuff =client->readAll();
    QString tmp=QString("Recv requestInfo from [%1:%2] ").arg(socketIpAddress).arg(port);
    qDebug()<<tmp;
    qDebug()<<recvBuff;
    //任务调度
    Dispatcher * d=new Dispatcher(client,this);
    QJsonObject requestInfo=QJsonDocument::fromJson(recvBuff).object();
    QJsonObject responseInfo =d->dispatch(requestInfo);
    //返回responseInfo
    sendMsgToClients(responseInfo,client);
    delete d;
}
//检测client连接状态变化
void Server::socketStateChanged(QAbstractSocket::SocketState state)
{
    QTcpSocket* client = qobject_cast<QTcpSocket*>(QObject::sender());
    QString socketIpAddress = client->peerAddress().toString();
    int port = client->peerPort();
    QString desc;
    // simply print out a relevant message according to its new state
    if (state == QAbstractSocket::UnconnectedState)
        desc = "The socket is not connected.";
    else if (state == QAbstractSocket::HostLookupState)
        desc = "The socket is performing a host name lookup.";
    else if (state == QAbstractSocket::ConnectingState)
        desc = "The socket has started establishing a connection.";
    else if (state == QAbstractSocket::ConnectedState)
        desc = "A connection is established.";
    else if (state == QAbstractSocket::BoundState)
        desc = "The socket is bound to an address and port.";
    else if (state == QAbstractSocket::ClosingState)
        desc = "The socket is about to close (data may still be waiting to be written).";
    else if (state == QAbstractSocket::ListeningState)
        desc = "For internal use only.";
    qDebug() << "Socket state changed [" + socketIpAddress + ":" + QString::number(port) + "]: " + desc;
}
//判断用户名为userName的用户是否在线
bool Server::isOnline(QString userName) const{
    for(auto name:onlineUser){
        if(name == userName)
            return true;
    }
    return false;
}
//将在线用户添加到在线用户列表
void Server::addOnlineUser(QString userName, QTcpSocket *socket){
    onlineUser.push_back(userName);
    onlineUserSocket.push_back(socket);
}
//将离线用户从在线列表中删除,利用userName或socket
void Server::deleteOfflineUser(QString userName,QTcpSocket *socket){
    for(int i=0;i<onlineUser.size()&&i<onlineUserSocket.size();i++){
        if(onlineUser.at(i) == userName||onlineUserSocket.at(i)==socket){
            onlineUser.removeAt(i);
            onlineUserSocket.removeAt(i);
            return ;
        }
    }
}
void Server::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap pix(":/pic/bg.jpg");
    painter.drawPixmap(-width()*0.5,-height()*0.1,pix.width(),pix.height(),pix);
}
QList<QString> Server::getOtherUser()const{
    QList<QString> ans;
    QSqlQuery query(db);
    query.exec(QString("select * from user"));
    while(query.next()){
        QString userName= query.value(0).toString();
        if(onlineUser.contains(userName))
            continue;
        ans.append(userName);
        qDebug()<<"offLine User:"<<userName;
    }
    return ans;
}
