#include "client.h"
#include<QHostAddress>
#include"define.h"
Client::Client(QWidget *parent) :
    QWidget(parent)
{
    isConnected=false;
    qDebug()<<"Client(), set a new socekt";
}

Client::~Client(){
    socketDisconnect();
    socket->close();
    qDebug()<<"~Client(), socket is closed";
 }
void Client::socketConnect(char *addr){
    if(!isConnected){//没有连接服务器
        //尝试连接服务器
        socket=new QTcpSocket();
        //连接槽函数
        connect(socket,&QTcpSocket::connected,[=](){
         qDebug()<<"Connect to server.";
         isConnected=true;

        });
        connect(socket,&QTcpSocket::disconnected,[=](){
         qDebug()<<"Disconnect from server.";
         isConnected=false;

        });
        connect(socket,&QTcpSocket::readyRead,[=](){
            recvBuff=socket->readAll();
            qDebug()<<"Recv responseInfo from server.";
            qDebug()<<recvBuff;
        });
        qDebug()<<"Try to connect server....";
        socket->connectToHost(addr,port);
        socket->waitForConnected(1000);//等待连接建立
    }
}
void Client::socketDisconnect(){
    if(isConnected){//已经连接到服务器了
        //断开连接
        socket->disconnectFromHost();
        socket->close();
    }
}
QJsonObject Client::sendMsg(QJsonObject requestInfo){
    QJsonObject responseInfo;
    if(isConnected){
        QJsonDocument doc(requestInfo);
        socket->write(doc.toJson());
        qDebug()<<"Send requestInfo to server.";
        qDebug()<<doc.toJson();
        if(socket->waitForReadyRead(1000)){//等待服务器返回信息
            return QJsonDocument::fromJson(recvBuff).object();
            
        }
    }
    responseInfo.insert("TYPE",SERVER_ERROR);
    return responseInfo;
}

