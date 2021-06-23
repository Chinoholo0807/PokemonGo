#include"logwidget.h"
#include"hallwidget.h"
#include"ui_logwidget.h"
#include"define.h"
#include<QJsonObject>
#include<QJsonDocument>
#include<QMessageBox>
LogWidget::LogWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //连接按钮和对应槽函数
    setWindowTitle("Pokeman 客户端");
    setWindowIcon(QIcon(":/res/icon.png"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    ui->lineEditServerAddr->setText(QString("127.0.0.1"));
    connect(ui->pushButtonLogin,&QPushButton::clicked,this,&LogWidget::slotPushButtonLogin);
    connect(ui->pushButtonRegister,&QPushButton::clicked,this,&LogWidget::slotPushButtonRegister);
    //连接到服务器


}

LogWidget::~LogWidget()
{
    delete ui;
}
void LogWidget::slotPushButtonLogin(){
    qDebug()<<"Press pushButtonLogin";
    //获取用户名和密码
    QString userName =ui->lineEditUserName->text();
    QString password=ui->lineEditPassword->text();
    //向服务器发送登录请求
    qDebug()<<"Create new client";
    client= new Client();//创建新用户
    client->socketConnect(ui->lineEditServerAddr->text().toLatin1().data());
    if(!client->connected()){
        QMessageBox::information(this,"登录失败","服务器连接失败");
        delete client;
        return ;
    }
    client->setUserName(userName);//设置用户名
    //向服务器发送数据
    QJsonObject requestInfo;
    requestInfo.insert("type",LOGIN);
    requestInfo.insert("user_name",userName);
    requestInfo.insert("password",password);
    QJsonObject responseInfo=client->sendMsg(requestInfo);
    switch(responseInfo.value("type").toInt()){
    case LOGIN_WP:
    {
        //登录失败，用户名错误或密码错误
        delete client;
        QMessageBox::information(this,"登录失败","用户名错误或密码错误");
        break;
    }
    case LOGIN_OL:
    {
        //登陆失败，当前用户已在线
        delete client;
        QMessageBox::information(this,"登录失败","当前用户已在线");
        break;
    }
    case LOGIN_SC:
    {
        //登录成功，进入大厅
        this->close();//关闭登录窗口
        //默认进入大厅窗口
        StackWidget *stackWidget = new StackWidget(client);
        stackWidget->show();
        break;
    }
    default:{
        delete client;
        QMessageBox::information(this,"登录失败",QString("未知错误#%1").arg(responseInfo.value("type").toInt()));
        break;
    }
    }

}
void LogWidget::slotPushButtonRegister(){
    qDebug()<<"Press pushButtonRegister";
    //获取用户名和密码
    QString userName =ui->lineEditUserName->text();
    QString password=ui->lineEditPassword->text();
    //向服务器发送注册请求
    qDebug()<<"Create new client";
    client= new Client();//创建新用户

    client->socketConnect(ui->lineEditServerAddr->text().toLatin1().data());
    if(!client->connected()){
        QMessageBox::information(this,"注册失败","服务器连接失败");
        delete client;
        return ;
    }
    //向服务器发送数据
    QJsonObject requestInfo;
    requestInfo.insert("type",REGISTER);
    requestInfo.insert("user_name",userName);
    requestInfo.insert("password",password);
    QJsonObject responseInfo=client->sendMsg(requestInfo);
    switch(responseInfo.value("type").toInt()){
    case REGISTER_WP:
    {
        //注册失败
        delete client;
        QMessageBox::information(this,"注册失败","用户名已存在");
        break;
    }
    case REGISTER_SC:
    {
        //注册成功
        delete client;
        QMessageBox::information(this,"注册成功","请使用新注册的用户登录");
        break;
    }
    default:
    {
        delete client;
        QMessageBox::information(this,"注册失败",QString("未知错误#%1").arg(responseInfo.value("type").toInt()));
        break;
    }
    }
}
